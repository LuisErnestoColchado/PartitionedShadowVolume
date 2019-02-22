#ifndef APP_H_
#define APP_H_

#include "include.h"
#include "object.h"

class app{

public:

    app(std::vector<object*>,
        int);
    const char * computeFile;
    const char * fragmentFile;
    std::vector<object*> objects;

    int factor= 0.0;

    void setShadersBuild(const char *);

    void getTriangles();

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

    triangle triangles[2000000];
    node nodes[(2000000+1)*4];
    triangle trianglesc[(2000000+1)*4];;

    GLuint TOPTREE;
    GLuint rootGL;
    GLuint trianglesBuffer;
    glm::vec4 lightPos;
    glm::vec3 colores[2];
    GLuint programBuild;
    GLuint lightID;
    GLuint colorID;
    glm::uint64 root = 0;
    uint64_t sizeTriangles = 0;
    uint64_t sizeNodes = 0;

};

#endif
