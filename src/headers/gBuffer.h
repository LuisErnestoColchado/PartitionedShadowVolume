#ifndef GBUFFER_H_
#define GBUFFER_H_

#include "include.h"

class gBuffer
{
public:

    gBuffer(GLuint position,GLuint normal,GLuint deferredTexture);

    ~gBuffer();

    void buildBuffer();

private:

    GLuint position;
    GLuint normal;
    GLuint deferredTexture;
    
};

#endif