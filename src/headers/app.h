#ifndef APP_H_
#define APP_H_

#include "include.h"
#include "object.h"

class app{

public:

    app(const char *,const char *, std::vector<object*>);
    const char * computeFile;
    const char * fragmentFile;
    std::vector<object*> objects;

    int factor= 0.0;
    GLuint VertexArrayID;
    GLuint programBuild;
    GLuint programFragment;
    GLuint matrixID;
    GLuint viewMatrixID;
    GLuint modelMatrixID;
    GLuint textureID;
    GLuint lightID;
    GLuint sizeBufferID;

    GLuint vertexBuffer;
    GLuint uvBuffer;
    GLuint normalBuffer;
    GLuint trianglesBuffer;
    GLuint TOPtree;
    GLuint rootGL;

    void setShadersBuild();

    void setShadersRender();

    void buildingTOPtree();

    void rendering();

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
    uint64_t root;
    uint64_t sizeTriangles;
    uint64_t sizeNodes;

    std::vector<glm::vec4> totalVertices;
    std::vector<glm::vec2> totalUvs;
    std::vector<glm::vec3> totalNormals;

    glm::vec4 lightPos;
    glm::mat4 modelMatrix;
    glm::mat4 MVP;
    glm::mat4 viewMatrix;

};

#endif
