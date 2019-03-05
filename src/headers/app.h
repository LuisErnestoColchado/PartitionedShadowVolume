#ifndef APP_H_
#define APP_H_

#include "include.h"
#include "object.h"
//rex 112596
//robot 171452
//machine 134777
//raptor 2000000
#define SIZEOFTRIANGLE 2000000
#define ALPHA 0.01;
#define BETA 0.01;
class app{
public:
    struct triangle {
        glm::vec4 a;
        glm::vec4 b;
        glm::vec4 c;
    };

    struct node {
        glm::vec4 plane;
        uint link[4]; /* 0: positive child, 1: intersection child, 2: negative child (not used), 3: wedge angle */
    };

    struct util{
        uint node = 0;
        uint triangle = 0;
    } variable;
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
    void cleanBuffers();
    void improvePSV(triangle *);
    int calculateDistance(glm::vec3, glm::vec3, glm::vec3, glm::vec4);
    glm::vec4 computePlane(glm::vec3,glm::vec3);

    //112596
    triangle triangles[SIZEOFTRIANGLE];
    node nodes[SIZEOFTRIANGLE*4+1];

    //triangle * triangles;
    //node * nodes;
    GLuint TOPTREE;
    GLuint rootGL;
    GLuint utilGL;
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
