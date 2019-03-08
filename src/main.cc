#include <headers/include.h>
#include <headers/object.h>
#include <headers/app.h>
#include <chrono>
#include <ctime>

GLFWwindow* window;
std::vector<object*> objects;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
struct triangle {
    glm::vec4 a;
    glm::vec4 b;
    glm::vec4 c;
};
int main(int argc, char ** argv) {

    int option = std::stoi(argv[1]);

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
    const char * skeleton = "../data/skeleton.obj";
    const char * machineFile = "../data/machine.obj";
  	const char * raptorFile = "../data/raptor.obj";
    const char * venusFile = "../data/venusl.obj";
    const char * ateneaFile = "../data/ateneal.obj";
    const char * elephamFile = "../data/elephal.obj";
  	const char * plataformFile = "../data/Empty.obj";
		const char * textureFileRaptor = "../data/Bone.bmp";
    const char * textureFilePlata = "../data/cueva.bmp";

		int sizeTriangle = 0;

    object* obj3D = new object(elephamFile,1.0,true,sizeTriangle,textureFileRaptor);

    std::cout << "size triangles " <<  sizeTriangle << std::endl;

    object* obj3Dp = new object(plataformFile,1.0,false,sizeTriangle,textureFilePlata);

    obj3D->buildBuffers();
    obj3Dp->buildBuffers();

	  obj3D->setShaders("../shaders/VS_interpolation.glsl","../shaders/FS_queryPoint.glsl");
		obj3Dp->setShaders("../shaders/VS_interpolation.glsl","../shaders/FS_queryPoint.glsl");

    objects.push_back(obj3D);
    objects.push_back(obj3Dp);

    app * a = new app(objects,sizeTriangle);
    a->getTriangles();

    a->setShadersBuild("../shaders/CS_buildingTopTree.glsl");

    int countFrame = 0;

    //a->cleanBuffers();
    //bool flag = false;
    if(option == 2){
        a->improvePSV();
    }
    a->buildingTOPtree();

    int countBuilding = 0;

    bool flag = false;
    std::cout << "===================================\n";
  	do{

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        if(countFrame % 100 == 0){
            a->buildingTOPtree();

        }
        a->rendering();

    		/* Swap front & back buffers */
    		glfwSwapBuffers(window);

    		/* Manage events */
    		glfwPollEvents();

        countFrame++;

    }while( countFrame < 700 && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
  			glfwWindowShouldClose(window) == 0 );
    if(option == 2){
        std::cout << "Building Time " << (a->buildingTime+a->timeImprovePSV) << std::endl;
    }
    else{
        std::cout << "Building Time " << a->buildingTime << std::endl;
    }

    std::cout << "Rendering Time " << a->fragmentTime/countFrame << std::endl;
  	glfwTerminate();
  	return EXIT_SUCCESS;

}
