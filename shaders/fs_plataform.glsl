#version 430 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

in vec3 texCoord;
// datos de salida hacia el fragment shader (lo que tenemos que calcular)
// datos unifromes a todo el objeto
uniform sampler2D myTextureSampler;

uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
// a TOP tree node
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


/* return 0 if p is in shadow, 1 otherwise
	p is the fragment in the world coordinates system with the light as origin
	normal is the surface normal at p
*/
float TOPTREE_query( in vec3 p, in vec3 normal ){ 
	// 32 uint stack to push intersection node. 32 is the value used in the EG2015 paper for all tests on hard shadows
    uint stack[32]; 
	uint stacksize = 0;
	const float sqdist = dot(p,p); // squared distance from p to the origin (the light)

	stack[ stacksize++ ] = root; // push the root
	
	// if we are back facing the light, querying the TOP tree is not necessary
	if ( dot( normal, -p) < 0.0 ) return 0.0;
 	
 	// find the location of p...
	while(stacksize>0){
		// pop 
		const uint current = stack[ --stacksize ];
		const node n = nodes[ current ];

		// compute the signed distance from p to the current plane
		const float offset = dot(n.plane, vec4(p,1));

		// if an intersection child exists and if current is a capping plane or if it is a shadow plane and p is inside its wedge 
		if ( n.link[1]>0 && (current%4==3 || offset*offset / sqdist < uintBitsToFloat(n.link[3])) )
		 		stack[ stacksize++ ] = n.link[1];

		// if p is in the positive halfspace of the plane
		if ( offset>0.0 ){
			// the location of p continues in the positive child if it exists
			if ( n.link[0]>0 ) stack[ stacksize++ ] = n.link[0];
		}
		// otherwise p is in the negative halfspace
		else { 
			// if the negative child is a leaf, p is inside a SV and thus it is not visible from the light
			if ( current%4==3 ) return 0.0; 
			// otherwise the localisation of p continues in the negative child
			else stack[ stacksize++ ] = current+1;
		}		
	}
	return 1.0;
}

// must return the fragment position in the world space coordinates system
vec3 getFragmentPosition(){
	// coordinates should not be interpolated !
	return vertexPosition_modelspace.xyz;
}

// must return the fragment normal in the world coordinates system
vec3 getFragmentNormal(){
	return normalize(vertexPosition_modelspace).xyz;
}

// return true if a fragment exists, otherwise false (no projected geometry)
/*bool fragmentExist( in vec4 frag ){
	// TO COMPLETE 
}*/

// must return the light position in the world space coordinates system
vec3 getLight(){
	return LightPosition_worldspace;
}
out vec3 color;

void main(){

	// Material del objeto tb deberian de ser uniformes
	vec3 MaterialDiffuseColor = vec3(0.2,0.2,0.2);//texture( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(1.0,1.0,1.0);

	//frosbite / cryengine
	vec3 LightColor = vec3(1.0f,1.0f,1.0f);

	//Unreal
	//vec3 LightColor = vec3(1.0f,1.0f,1.0f)*pow(4.0,2)/(pow(distance,2)+0.1f);

	// Normal despues de la interpolacion
	vec3 n = normalize(Normal_cameraspace);
	// Direccion de la luz : fragment -> luz antes de interpolacion era vertex -> luz
	vec3 l = normalize( LightPosition_worldspace );

	// Coseno del angulo entre la normal y la luz
	// Producto punto ya que los vectos son normalizados
	// clamped  0 - 1
	//  - luz vertical -> 1
	//  - luz perpandicular -> 0
	//  - luz detras de triangulo -> 0
	float cosTheta = clamp(dot(n,l),0,1);
	//Codigo aqui
	
	// vector fragmento -> camera antes vertex -> camera
	vec3 E = normalize(EyeDirection_cameraspace);
	// reflejamos el vector Descarte (espejo perfecto)

	vec3 r = reflect(-l,n);
	//Codigo aqui
		//vec4 pos = getFragmentPosition();
	//if( fragmentExist(pos)==true )
	//{
		//vec3 normal = getFragmentNormal();
		//vec3 light = getLight();
		//float visibility = TOPTREE_query(pos.xyz-light, normal);
	//	vec3 n = normalize( normal );
	// Direccion de la luz : fragment -> luz antes de interpolacion era vertex -> luz
	//	vec3 l = normalize( light );
	//	vec3 r = reflect(-l,n);
	// Coseno del angulo entre la normal y la luz
	// Producto punto ya que los vectos son normalizados
	// clamped  0 - 1
	//  - luz vertical -> 1
	//  - luz perpandicular -> 0
	//  - luz detras de triangulo -> 0
	//    float cosTheta = clamp(dot(n,l),0,1);
	//	Codigo aqui
	float cosAlpha = clamp(dot(r,E),0,1); 
	// 	vector fragmento -> camera antes vertex -> camera
	//	vec3 E = normalize(EyeDirection_cameraspace);
	// 	reflejamos el vector Descarte (espejo perfecto)
	// You may want compute the final color using visiblity (0/1), light, normal and pos ! Or whatever you want.

	//}
	//else
	//	color=vec4(1);
	// coseno entre luz reflejada y la direccion hacia la camara
	// clamped  0-1
	//  - Miramos en la reflexion -> 1
	//  - nos alejamos de la reflexion -> < 1
	 
		//float cosAlpha = clamp(dot(r,E),0,1); 
	if(cosTheta > 0.9){
		color = MaterialDiffuseColor;
	}
	else if(cosTheta > 0.6){
		color = MaterialDiffuseColor * vec3(0.3,0.3,0.3 );
	}
	else{
		color = MaterialDiffuseColor;
	}

	if(cosAlpha > 0.95){
		color = MaterialSpecularColor;
 	}
    
}