#include "headers/object.h"

object::object(){}

object::object(const char* filename,
            float scale,
            bool gShadow,
            int &sizeTriangles,
            const char * textureFile){
    glGenVertexArrays(1, &VertexArrayID);
    meshFilename = filename;
    load_scale = scale;
    modelMatrix = glm::mat4(1.0);
    generateShadow = gShadow;
    modelMatrix = glm::mat4(1.0);

    texture = loadBMP_custom(textureFile);


    loadMesh(NULL,true,sizeTriangles);
}

object::~object(){

}

void object::setShaders(const char * vFile,
                        const char * fFile){
    programRender = LoadShaders( vFile, fFile);

  	// Get a handle for our "MVP" uniform
  	matrixID = glGetUniformLocation(programRender, "MVP");
  	viewMatrixID = glGetUniformLocation(programRender, "V");
  	modelMatrixID = glGetUniformLocation(programRender, "M");
  	textureID  = glGetUniformLocation(programRender, "myTextureSampler");
  	lightID = glGetUniformLocation(programRender, "LightPosition_worldspace");
    colorID = glGetUniformLocation(programRender, "matColor");
    sizeBufferID = glGetUniformLocation(programRender,"sizeBuffer");
}


bool object::loadMesh(const char* basepath,
                        bool triangulate,
                        int &sizeTriangles){
    basepath = NULL;
    triangulate = true;
    std::cout << meshFilename << std::endl;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string error;

    bool ret = tinyobj::LoadObj(&attrib,
                                &shapes,
                                &materials,
                                &warnings,
                                &error,
                                meshFilename,
                                basepath,
                                triangulate);

    if (!warnings.empty()) {
        std::cout << "WARN: " << warnings << std::endl;
    }

    if (!error.empty()) {
        std::cerr << "ERR: " << error << std::endl;
    }

    if (!ret) {
        printf("Failed to load/parse .obj.\n");
        return false;
    }

    std::vector<glm::vec4> vert;
    std::vector<glm::vec2> text;
    std::vector<glm::vec3> normal;
    glm::vec4 vec;
    for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        vec = glm::vec4(double(attrib.vertices[3 * v + 0]),double(attrib.vertices[3 * v + 1]),double(attrib.vertices[3 * v + 2]),1);
        vert.push_back(vec);
    }

    for(int i = 0;i < attrib.texcoords.size();i+=2){
        glm::vec2 vec = glm::vec2(attrib.texcoords[i],attrib.texcoords[i+1]);
        text.push_back(vec);
    }

    for(int i = 0;i < attrib.normals.size();i+=3){
        glm::vec3 vec = glm::vec3(float(attrib.normals[i]),float(attrib.normals[i+1]),float(attrib.normals[i+2]));
        normal.push_back(vec);
    }
    int value = vert.size() % 3;

    for (auto x : shapes){
      assert((x.mesh.indices.size() % 3) == 0);
      std::cout << x.mesh.indices.size()/3 << std::endl;
      int count = 0;
      for (size_t f = 0; f < x.mesh.indices.size() / 3; f++) {
          tinyobj::index_t i0 = x.mesh.indices[3 * f + 0];
          tinyobj::index_t i1 = x.mesh.indices[3 * f + 1];
          tinyobj::index_t i2 =x.mesh.indices[3 * f + 2];

          vertices.push_back(vert[i0.vertex_index]);
          vertices.push_back(vert[i1.vertex_index]);
          vertices.push_back(vert[i2.vertex_index]);

          if(i0.texcoord_index > 0 && i0.texcoord_index < text.size()){
              uvs.push_back(text[i0.texcoord_index]);
              uvs.push_back(text[i1.texcoord_index]);
              uvs.push_back(text[i2.texcoord_index]);
          }
          
          normals.push_back(normal[i0.normal_index]);
          normals.push_back(normal[i1.normal_index]);
          normals.push_back(normal[i2.normal_index]);
          count++;
      }
    }
    std::cout << meshFilename << " " << text.size() << std::endl;
    if(generateShadow == true)
        sizeTriangles += vertices.size() / 3;

    return true;
}
void object::buildBuffers(){
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);
    glUnmapBuffer( GL_ARRAY_BUFFER );

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);
    glUnmapBuffer( GL_ARRAY_BUFFER );

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glUnmapBuffer( GL_ARRAY_BUFFER );
}
