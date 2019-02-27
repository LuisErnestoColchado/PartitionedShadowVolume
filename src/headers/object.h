#ifndef OBJECT_H_
#define OBJECT_H_

#include "include.h"

class object{
public:
    object();
    object(const char* filename,
            float scale,
            bool gShadow,
            int &sizeTriangle,
            const char * textureFile);
    object(std::vector<glm::vec3> vecs,
            std::vector<glm::vec2> uv,
            std::vector<glm::vec3> normal);
    ~object();

    GLuint VertexArrayID;

    GLuint programRender;

    GLuint matrixID;
    GLuint viewMatrixID;
    GLuint modelMatrixID;
    GLuint textureID;
    GLuint lightID;
    GLuint sizeBufferID;

    GLuint vertexBuffer;
    GLuint uvBuffer;
    GLuint normalBuffer;

    GLuint texture;

    const char* meshFilename;
    bool loadMesh(const char*,
                    bool,
                    int&);
    void buildBuffers();
    void setShaders(const char*,
                    const char *);

    float load_scale;

    glm::vec4 lightRotation;
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    int generateShadow;
    GLuint colorID;
    glm::mat4 modelMatrix;
    glm::mat4 MVP;
    glm::mat4 viewMatrix;
};
#endif
