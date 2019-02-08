#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.h"

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	std::cout << "shader render 1" << std::endl;
	GLuint ProgramID = glCreateProgram();
	std::cout << "shader render 2" << std::endl;
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	std::cout << "shader render 3" << std::endl;
	glLinkProgram(ProgramID);
	std::cout << "shader render 4" << std::endl;
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
			
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
GLuint LoadShadersBuild(const char * compute_file_path){
	
	// Create the shaders
	GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);
	std::cout << "step compute 1" << std::endl;
	// Read the Vertex Shader code from the file
	std::string ComputerShaderCode;
	std::ifstream ComputeShaderStream(compute_file_path, std::ios::in);
	if(ComputeShaderStream.is_open()){
		std::string Line = "";
		while(getline(ComputeShaderStream, Line))
			ComputerShaderCode += "\n" + Line;
		ComputeShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", compute_file_path);
		getchar();
		return 0;
	}
	// Compile compute Shader
	printf("Compiling shader : %s\n", compute_file_path);
	char const * FragmentSourcePointer = ComputerShaderCode.c_str();
	glShaderSource(ComputeShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(ComputeShaderID);

	std::cout << "step compute 2" << std::endl;
	GLuint ComputeShaderProgID = glCreateShaderProgramv(GL_COMPUTE_SHADER, 1, (const char**)&ComputerShaderCode);
	GLint Result = GL_FALSE;
	int InfoLogLength;
	std::cout << "step compute 3" << std::endl;
	glAttachShader(ComputeShaderProgID, ComputeShaderID);
	glLinkProgram(ComputeShaderProgID);
	// Check the program
	glGetProgramiv(ComputeShaderProgID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ComputeShaderProgID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ComputeShaderID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDetachShader(ComputeShaderProgID, ComputeShaderID);
	
	glDeleteShader(ComputeShaderID);
	std::cout << "step compute 4" << std::endl;
	glCompileShader(ComputeShaderProgID);
	return ComputeShaderProgID;
}

GLuint LoadShadersRender2(const char * fragment_file_path){
	
	// Create the shaders
	GLuint FragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "step compute 1" << std::endl;
	// Read the Vertex Shader code from the file
	std::string ComputerShaderCode;
	std::ifstream ComputeShaderStream(fragment_file_path, std::ios::in);
	if(ComputeShaderStream.is_open()){
		std::string Line = "";
		while(getline(ComputeShaderStream, Line))
			ComputerShaderCode += "\n" + Line;
		ComputeShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragment_file_path);
		getchar();
		return 0;
	}
	// Compile compute Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = ComputerShaderCode.c_str();
	glShaderSource(FragmentID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentID);

	std::cout << "step compute 2" << std::endl;
	GLuint FragmentShaderProgID = glCreateShaderProgramv(GL_COMPUTE_SHADER, 1, (const char**)&ComputerShaderCode);
	GLint Result = GL_FALSE;
	int InfoLogLength;
	std::cout << "step compute 3" << std::endl;
	glAttachShader(FragmentShaderProgID, FragmentID);
	glLinkProgram(FragmentShaderProgID);
	// Check the program
	glGetProgramiv(FragmentShaderProgID, GL_LINK_STATUS, &Result);
	glGetProgramiv(FragmentShaderProgID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(FragmentID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDetachShader(FragmentShaderProgID, FragmentID);
	
	glDeleteShader(FragmentID);
	std::cout << "step compute 4" << std::endl;
	glCompileShader(FragmentShaderProgID);
	return FragmentShaderProgID;
}


