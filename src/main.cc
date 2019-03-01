#include <headers/include.h>
#include <headers/object.h>
#include <headers/app.h>
#include <chrono>
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
    const char * rexFile = "../data/T-rex-skeleton_21.obj";
    const char * robotFile = "../data/androrobot.obj";
    const char * machineFile = "../data/machine.obj";
  	const char * raptorFile = "../data/raptor.obj";
  	const char * plataformFile = "../data/Empty.obj";
		const char * textureFileRaptor = "../data/Bone.bmp";
    const char * textureFilePlata = "../data/cueva.bmp";
		int sizeTriangle = 0;

    object* obj3D = new object(raptorFile,1.0,true,sizeTriangle,textureFileRaptor);
    std::cout << "size triangles " <<  sizeTriangle << std::endl;
		//obj3D->modelMatrix = glm::rotate(obj3D->modelMatrix,glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f));
		//RAPTOR
    //obj3D->modelMatrix = glm::rotate(obj3D->modelMatrix,glm::radians(-30.0f),glm::vec3(0.0f,1.0f,0.0f));
    //obj3D->modelMatrix = glm::translate(obj3D->modelMatrix,glm::vec3(0.2f,0.19f,-2.2f));
    //obj3D->modelMatrix = glm::translate(obj3D->modelMatrix,glm::vec3(0.0f,0.0f,10.0f));
    object* obj3Dp = new object(plataformFile,1.0,false,sizeTriangle,textureFilePlata);
    //obj3Dp->modelMatrix = glm::translate(obj3D->modelMatrix,glm::vec3(0.0f,-0.1f,2.7f));

    obj3D->buildBuffers();
    obj3Dp->buildBuffers();
	  obj3D->setShaders("../shaders/vs_mapscreen.glsl","../shaders/fs_psv.glsl");
		obj3Dp->setShaders("../shaders/vs_mapscreen.glsl","../shaders/fs_psv.glsl");
    objects.push_back(obj3D);
    objects.push_back(obj3Dp);


    app * a = new app(objects,sizeTriangle);
    a->getTriangles();
    a->setShadersBuild("../shaders/cs_simple.glsl");
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    a->buildingTOPtree();
    end = std::chrono::system_clock::now();
    int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    std::cout << "time " << elapsed_seconds << std::endl;
    int countFrame = 0;
    //a->setShadersRender();
  	do{
      std::chrono::time_point<std::chrono::system_clock> start, end;
      start = std::chrono::system_clock::now();
  		/* Update and render one frame */
  		//AppFrame();
  		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  		// Compute the MVP matrix from keyboard and mouse input
  		computeMatricesFromInputs();

      if (glfwGetKey(window, GLFW_KEY_ENTER ) == GLFW_PRESS)
          a->buildingTOPtree();

      a->rendering();
      //a->cleanBuffers();
  		/* Swap front & back buffers */
  		glfwSwapBuffers(window);

  		/* Manage events */
  		glfwPollEvents();
      end = std::chrono::system_clock::now();
      elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                               (end-start).count();
      std::cout << "time F" << elapsed_seconds << std::endl;
      countFrame++;
    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
  			glfwWindowShouldClose(window) == 0 );

  	glfwTerminate();
  	return EXIT_SUCCESS;

}
