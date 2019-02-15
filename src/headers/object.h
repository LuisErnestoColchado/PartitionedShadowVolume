#ifndef OBJECT_H_
#define OBJECT_H_

#include "include.h"

class object{
public:
    object();
    object(const char* filename, float scale=1.0);
    object(std::vector<glm::vec3> vecs, std::vector<glm::vec2> uv, std::vector<glm::vec3> normal);
    ~object();

    const char* meshFilename;
    bool loadMesh(const char* basepath,
                    bool triangulate);
    float load_scale;
    
    glm::mat4 modelMatrix;
    glm::vec4 lightPos;
    glm::vec4 lightRotation;
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    struct triangle {
        glm::vec4 a;
        glm::vec4 b;
        glm::vec4 c;
    };

    struct node {
        glm::vec4 plane;
        uint link[4]; /* 0: positive child, 1: intersection child, 2: negative child (not used), 3: wedge angle */
    };
    triangle *triangles;
    node *nodes;
    uint32_t sizeTriangles;
    uint32_t sizeNodes;  
    
    GLint texture;
};
#endif 