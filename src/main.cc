#include <headers/include.h>
#include <headers/object.h>
#include <headers/app.h>

GLFWwindow* window;
std::vector<object*> objects;

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
  	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
  	const char * raptorFile = "../data/raptor.obj";
  	const char * plataformFile = "../data/Empty.obj";

    object* obj3D = new object(raptorFile,1.0);
    obj3D->modelMatrix = glm::rotate(obj3D->modelMatrix,glm::radians(-30.0f),glm::vec3(0.0f,1.0f,0.0f));
    obj3D->modelMatrix = glm::translate(obj3D->modelMatrix,glm::vec3(0.2f,0.19f,-2.2f));
    objects.push_back(obj3D);

    object* obj3Dp = new object(plataformFile,1.0);
  std::cout << " 1 " << std::endl;
    objects.push_back(obj3Dp);

    app * a = new app("../shaders/cs_simple.glsl","../shaders/fs_psv.glsl",objects);
      std::cout << " 2 " << std::endl;
    a->setShadersBuild();
      std::cout << " 3 " << std::endl;
    a->setShadersRender();

  	do{
  		/* Update and render one frame */
  		//AppFrame();
  		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  		// Compute the MVP matrix from keyboard and mouse input
  		computeMatricesFromInputs();
      std::cout << " 2 " << std::endl;
      a->buildingTOPtree();
      a->rendering();
      std::cout << " 3 " << std::endl;
  		/* Swap front & back buffers */
  		glfwSwapBuffers(window);

  		/* Manage events */
  		glfwPollEvents();

    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
  			glfwWindowShouldClose(window) == 0 );

  	glfwTerminate();
  	return EXIT_SUCCESS;

}
