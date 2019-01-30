#ifndef OBJECT_H_
#define OBJECT_H_
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../ObjLoader/tiny_obj_loader.h"
#include "app.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../common/controls.h"
#include "../common/shader.h"

class Object{
public:
    const char* meshFilename;
    

    GLuint Texture;
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint uvbuffer;

    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID;
    GLuint TextureHandler;
    GLuint VertexArrayID;
    GLuint LightID;

    glm::mat4 ModelMatrix;
    glm::vec4 lightPos;
    glm::vec4 lightRotation;
    float load_scale;
    int factor= 0.0;
    
    std::vector<glm::highp_vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    Object();
    Object(const char* filename, float scale=1.0);
    Object(std::vector<glm::vec3> vecs, std::vector<glm::vec2> uv, std::vector<glm::vec3> normal);
    ~Object();
    void setShadersRender(const char*,const char*);
    bool loadMesh(const char* basepath,
                    bool triangulate);
    //static void PrintInfo(const tinyobj::attrib_t& attrib,
     //               const std::vector<tinyobj::shape_t>& shapes,
      //              const std::vector<tinyobj::material_t>& materials,
        //            bool triangulate);                        
    void draw();

    
};
#endif 