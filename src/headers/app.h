#ifndef APP_H_
#define APP_H_

#include "include.h"

int factor= 0.0;
GLint programBuild;
GLint programFragment;
GLint matrixID;
GLint viewMatrixID;
GLint modelMatrixID;
GLint textureID;
GLint lightID;
GLint sizeBufferID;

void setShadersBuild(const char *compute);

void setShadersRender(const char *fragment);

void buildingTOPtree();

void rendering();

void draw()
#endif