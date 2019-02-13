#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "objectNoShadow.h"
#include "../common/shader.h"
#include "../common/texture.h"
#include "../common/controls.h"

#include "../common/vboindexer.h"
#define MAX_BUFFERSIZE  2048

Object3DN::Object3DN(){
	glGenVertexArrays(1, &VertexArrayID);
	Texture=0;
	ModelMatrix = glm::mat4(1.0);
	lightPos=glm::vec4(4,4,1,0);
}

Object3DN::Object3DN(const char* filename, float scale){
	glGenVertexArrays(1, &VertexArrayID);
	//Texture=0;
	ModelMatrix = glm::mat4(1.0);
	lightPos=glm::vec4(1,1,1,0);
	//Texture = loadBMP_custom("data/merc/textures/merc.bmp");
	Texture = 0;
	//Texture2 = loadBMP_custom("data/textu.bmp");
	meshFilename = filename;
	load_scale=scale;
	loadMesh(NULL,true);

}
Object3DN::Object3DN(std::vector<glm::vec3> vecs, std::vector<glm::vec2> uv, std::vector<glm::vec3> normal){
	glGenVertexArrays(1, &VertexArrayID);
	Texture = 0;
	ModelMatrix = glm::mat4(1.0);
	lightPos=glm::vec4(1,1,1,0);
	vertices = vecs;
	uvs = uv;
	normals = normal;
	glBindVertexArray(VertexArrayID);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

Object3DN::~Object3DN(){
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
}

void Object3DN::setShaders(std::string vfile,std::string ffile){

	programID = LoadShaders( vfile.c_str(), ffile.c_str());

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

}

bool Object3DN::loadMesh(const char* basepath = NULL,
                        bool triangulate = true){

	//Si no consiguen a compilar con el SDK FBX
	tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string error;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warnings, &error,
                                meshFilename, basepath, triangulate);

  	
    if (!warnings.empty()) {
        std::cout << "WARN: " << warnings << std::endl;
    }

    if (!error.empty()) {
        std::cerr << "ERR: " << error << std::endl;
    }

    if (!ret) {
        printf("Failed to load/parse .obj.\n");
        return false;
    }
    std::vector<glm::vec4> vert;
    std::vector<glm::vec2> text;
    std::vector<glm::vec3> normal;
    for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        glm::vec4 vec = glm::vec4(double(attrib.vertices[3 * v + 0]),double(attrib.vertices[3 * v + 1]),double(attrib.vertices[3 * v + 2]),1);
        vert.push_back(vec);
    }

    for(int i = 0;i < attrib.texcoords.size();i+=2){
        glm::vec2 vec = glm::vec2(attrib.texcoords[i],attrib.texcoords[i+1]);
        text.push_back(vec);
    }

    for(int i = 0;i < attrib.normals.size();i+=3){
        glm::vec3 vec = glm::vec3(float(attrib.normals[i]),float(attrib.normals[i+1]),float(attrib.normals[i+2]));
        normal.push_back(vec);
    }
    int value = vert.size() % 3;
    
    
    for (auto x : shapes){
      assert((x.mesh.indices.size() % 3) == 0);
      std::cout << x.mesh.indices.size()/3 << std::endl;
      int count = 0;
      for (size_t f = 0; f < x.mesh.indices.size() / 3; f++) {
          
          tinyobj::index_t i0 = x.mesh.indices[3 * f + 0];
          tinyobj::index_t i1 = x.mesh.indices[3 * f + 1];
          tinyobj::index_t i2 =x.mesh.indices[3 * f + 2];
          
          vertices.push_back(vert[i0.vertex_index]);
          vertices.push_back(vert[i1.vertex_index]);
          vertices.push_back(vert[i2.vertex_index]);
          
          //std::cout << "dddd" << std::endl;
          if(i0.texcoord_index > 0 && i0.texcoord_index < text.size()){
              uvs.push_back(text[i0.texcoord_index]);
              uvs.push_back(text[i1.texcoord_index]);
              uvs.push_back(text[i2.texcoord_index]);
          } 
            
          normals.push_back(normal[i0.normal_index]);
          normals.push_back(normal[i1.normal_index]);
          normals.push_back(normal[i2.normal_index]);
          count++;
      }
    }
	std::cout << "size vertice after 1 " << vertices.size() << std::endl;
	std::cout << "finish loading" << std::endl;

	glBindVertexArray(VertexArrayID);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}


void Object3DN::draw(){

	glUseProgram(programID);
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();

	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
	factor = 1.0;
	glm::mat4 lrot = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(0,1,0));

	lightPos =  lrot * lightPos;
	if(time % 200 == 0)
		lightPos.x += 0.1*factor;

	if(lightPos.x>7.0){
		factor=-1;
	}
	else if(lightPos.x<-7.0){
		factor=1;
	}
	//glm::mat4 lightM(1.0f);
	//lightM = glm::rotate(lightM, lightRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//lightM = glm::rotate(lightM, lightRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//lightM = glm::rotate(lightM, lightRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

	//glm::vec4 rotatedLightPos = lightM * lightPos;
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);
	
		// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	time +=1 ;
}