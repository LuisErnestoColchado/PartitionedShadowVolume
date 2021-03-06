#ifndef SHADER_HPP
#define SHADER_HPP
#include <GL/glew.h>
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
GLuint LoadShadersBuild(const char * compute_file_path);
GLuint LoadShadersRender2(const char * fragment_file_path);
#endif
