#version 430 core

// Aqui van los vertex buffer que mandamos al GPU
layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

// datos de salida hacia el fragment shader (lo que tenemos que calcular)
out vec2 UV;
out vec4 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;


// Datos uniformes al objeto
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

struct node {
		vec4 plane;
		uint link[4]; /* 0: positive child, 1: intersection child, 2: negative child (not used), 3: wedge angle */
};

// TOP tree buffer.
layout (std430, binding=13) restrict buffer TOPTree	{
		node nodes[];
};

// Buffer to read the root index
layout (std430, binding=29) restrict buffer TOPTreeRoot	{
		uint root;
};

void main(){

		// gl_position es la position del vertice despues de la proyeccion
		gl_Position =  (MVP * vertexPosition_modelspace);

		// UV no hacemos nada mas que interpolacion
		UV = vertexUV;

		// La posicion del vertice solamente despues de la transformacion espacial (rotacion)
		Position_worldspace = (M * vertexPosition_modelspace);
		//Position_worldspace = vertexPosition_modelspace;
		// Vector que va del vertice hacia la camara
		// En el espacio camara la posicion de la camara es 0,0,0
		vec3 vertexPosition_cameraspace = ( V * M * vertexPosition_modelspace).xyz;
		EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

		// Vector que va del vertice hacia la camara( espacio camara) No hay M porque no transformamos la luz.
		vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
		LightDirection_cameraspace = LightPosition_cameraspace - vertexPosition_cameraspace;

		// Normal despues de la transformacion
		Normal_cameraspace = ( transpose(inverse(V * M)) * vec4(vertexNormal_modelspace,0)).xyz;


}
