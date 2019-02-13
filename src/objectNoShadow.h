
#ifndef OBJECT3DN_H
#define OBJECT3DN_H

#include "../ObjLoader/tiny_obj_loader.h"
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Object3DN 
{ 
    // Access specifier 
public:

 Object3DN();
 Object3DN(const char* filename1, float scale=1.0);
 Object3DN(std::vector<glm::vec3> vecs, std::vector<glm::vec2> uv, std::vector<glm::vec3> normal);
 ~Object3DN();

 void setShaders(std::string vfile,std::string ffile);
 bool loadMesh(const char* basepath,
                    bool triangulate);
 void draw();

 std::string vertShaderFilename;
 std::string fragShaderFilename;
 const char* meshFilename;
 //std::string meshFilename2;
 std::string textureFilename;
//std::string textureFilename2;
int factor= 0.0;
int time = 0;
 std::vector<glm::vec3> vertices;
 std::vector<glm::vec2> uvs;
 std::vector<glm::vec3> normals;

 GLuint programID;
 GLuint MatrixID;
 GLuint ViewMatrixID;
 GLuint ModelMatrixID;
 GLuint TextureID;
 GLuint TextureHandler;
 GLuint VertexArrayID;
 GLuint LightID;

 GLuint Texture;
 GLuint vertexbuffer;
 GLuint normalbuffer;
 GLuint uvbuffer;

 glm::mat4 ModelMatrix;

 glm::vec4 lightPos;
  glm::vec4 lightRotation;
 float load_scale;
}; 

#endif