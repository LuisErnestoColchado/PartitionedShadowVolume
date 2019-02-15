#include "../headers/app.h"

void setShadersBuild(const char* compute){
    programBuild = LoadShadersBuild(compute);
    // Get a handle for our "MVP" uniform
    matrixID = glGetUniformLocation(programBuild, "MVP");
    viewMatrixID = glGetUniformLocation(programBuild, "V");
    modelMatrixID = glGetUniformLocation(programBuild, "M");
    textureID  = glGetUniformLocation(programBuild, "myTextureSampler");
    lightID = glGetUniformLocation(programBuild, "LightPosition_worldspace");
    sizeBufferID = glGetUniformLocation(programBuild,"sizeBuffer");
}

void setShaderRender(const char* fragment){
    programFragment = LoadShadersBuild(fragment);
    // Get a handle for our "MVP" uniform
    matrixID = glGetUniformLocation(programFragment, "MVP");
    viewMatrixID = glGetUniformLocation(programFragment, "V");
    modelMatrixID = glGetUniformLocation(programFragment, "M");
    textureID  = glGetUniformLocation(programFragment, "myTextureSampler");
    lightID = glGetUniformLocation(programFragment, "LightPosition_worldspace");
    sizeBufferID = glGetUniformLocation(programFragment,"sizeBuffer");
    
}