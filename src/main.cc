#include <stdlib.h>
#include <stdio.h>
// Include GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;
#include <iostream>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"

void window_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char * argv[]) {
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1920, 1080, "PSV", NULL, NULL);
	glfwSetWindowPos(window, 1500, 0);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1920/2, 1080/2);


	// Dark blue background
	glClearColor(0.125f, 0.152f, 0.16f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	/* Mainloop */  
	const char * raptorFile = "../data/T-rex-skeleton_2.obj";
	const char * plataFormfile = "../data/Empty.obj";
	Object* obj3D = new Object(raptorFile,1.0);
	obj3D->ModelMatrix = glm::rotate(obj3D->ModelMatrix,glm::radians(-30.0f),glm::vec3(0.0f,1.0f,0.0f));
	obj3D->ModelMatrix = glm::translate(obj3D->ModelMatrix,glm::vec3(1.6f,0.2,-2.2f));
	std::cout << "step main 1" << std::endl;
	obj3D->setShadersBuild("../shaders/cs_simple.glsl");
		std::cout << "step main 2" << std::endl;
	obj3D->setShadersRender("../shaders/vs_mapscreen.glsl","../shaders/fs_psv.glsl");
	std::cout << "step main 3" << std::endl;
	//ojo
	//Object* objp3D = new Object(plataFormfile,1.0);
	
	//objp3D->ModelMatrix = glm::rotate(obj3D->ModelMatrix,glm::radians(-180.0f),glm::vec3(1.0f,0.0f,0.0f));
	//objp3D->ModelMatrix = glm::rotate(obj3D->ModelMatrix,glm::radians(-180.0f),glm::vec3(0.0f,0.0f,1.0f));
	//objp3D->ModelMatrix = glm::rotate(obj3D->ModelMatrix,glm::radians(180.0f),glm::vec3(1.0f,0.0f,0.0f));
	//objp3D->ModelMatrix = glm::rotate(obj3D->ModelMatrix,glm::radians(-90.0f),glm::vec3(0.0f,0.0f,1.0f));
	
	//ojo
	//objp3D->setShadersRender("../shaders/vs_mapscreen.glsl","../shaders/fs_plataform.glsl");

	do{

		/* Update and render one frame */
		//AppFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Compute the MVP matrix from keyboard and mouse input
			computeMatricesFromInputs();	
		/* Draw OBJ 3D */
		//objp3D->draw();
		obj3D->draw();
		
		/* Swap front & back buffers */
		glfwSwapBuffers(window);

		/* Manage events */
		glfwPollEvents();
	}while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0 );

	glfwTerminate();
	return EXIT_SUCCESS;
}
