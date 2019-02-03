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
    
    // basic triangle data structure
    struct triangle {
        glm::vec4 a;
        glm::vec4 b;
        glm::vec4 c;
    };
    struct node {
	    glm::vec4 plane;
	    uint link[4]; /* 0: positive child, 1: intersection child, 2: negative child (not used), 3: wedge angle */
    };

    GLuint Texture;
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint uvbuffer;
    GLuint trianglesbuffer;
    GLuint TOPtree;
    GLuint rootgl;
    GLuint size;

    GLuint programID_render;
    GLuint programID_build;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID;
    GLuint TextureHandler;
    GLuint VertexArrayID;
    GLuint LightID;
    GLuint sizeBufferID;
    GLuint sizeNodeID;
    
    glm::mat4 ModelMatrix;
    glm::vec4 lightPos;
    glm::vec4 lightRotation;
    float load_scale;
    int factor= 0.0;
    uint32_t sizebuffer=2000000;
    uint32_t sizenodes=2000000*4;

    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    triangle triangles[2000000];
    node nodes[2000000*4];
    glm::uint64 root;

    Object();
    Object(const char* filename, float scale=1.0);
    Object(std::vector<glm::vec3> vecs, std::vector<glm::vec2> uv, std::vector<glm::vec3> normal);
    ~Object();
    void setShadersRender(const char*,const char*);
    void setShadersBuild(const char*);
    bool loadMesh(const char* basepath,
                    bool triangulate);
    //static void PrintInfo(const tinyobj::attrib_t& attrib,
     //               const std::vector<tinyobj::shape_t>& shapes,
      //              const std::vector<tinyobj::material_t>& materials,
        //            bool triangulate);                        
    void draw();

    
};
#endif 