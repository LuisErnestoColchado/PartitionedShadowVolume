#include <GL/glew.h>
#include "Object.h"
//#include <../common/objloader.h>
#include <assert.h>
#include "../common/texture.h"
#define MAX_BUFFERSIZE  2048


Object::Object(const char* filename, float scale){
    
    glGenVertexArrays(1, &VertexArrayID);
	  Texture=loadBMP_custom("../data/Bone.bmp");
    load_scale = scale;
	  ModelMatrix = glm::mat4(1.0);
    size = sizebuffer;
	  lightPos=glm::vec4(0.2,-1,1,0);
    meshFilename = filename; 
    loadMesh(NULL,true);
    //Texture = 0;
}

Object::~Object(){

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID_render);
	glDeleteProgram(programID_build);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

}

void Object::setShadersRender(const char* vertex, const char* fragment){
    std::cout << "render 1" << std::endl;
    programID_render = LoadShaders(vertex, fragment);
    std::cout << "render 2" << std::endl;
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID_render, "MVP");
    ViewMatrixID = glGetUniformLocation(programID_render, "V");
    ModelMatrixID = glGetUniformLocation(programID_render, "M");
    TextureID  = glGetUniformLocation(programID_render, "myTextureSampler");
    LightID = glGetUniformLocation(programID_render, "LightPosition_worldspace");
    sizeBufferID = glGetUniformLocation(programID_render,"sizeBuffer");
}

void Object::setShadersBuild(const char* compute){
    programID_build = LoadShadersBuild(compute);
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID_build, "MVP");
    ViewMatrixID = glGetUniformLocation(programID_build, "V");
    ModelMatrixID = glGetUniformLocation(programID_build, "M");
    TextureID  = glGetUniformLocation(programID_build, "myTextureSampler");
    LightID = glGetUniformLocation(programID_build, "LightPosition_worldspace");
    sizeBufferID = glGetUniformLocation(programID_build,"sizeBuffer");
    
}

void Object::setShader(const char* fragment){
    programID_fragment = LoadShadersBuild(fragment);
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID_fragment, "MVP");
    ViewMatrixID = glGetUniformLocation(programID_fragment, "V");
    ModelMatrixID = glGetUniformLocation(programID_fragment, "M");
    TextureID  = glGetUniformLocation(programID_fragment, "myTextureSampler");
    LightID = glGetUniformLocation(programID_fragment, "LightPosition_worldspace");
    sizeBufferID = glGetUniformLocation(programID_fragment,"sizeBuffer");
    
}


/*
void Object::PrintInfo(const tinyobj::attrib_t& attrib,
                      const std::vector<tinyobj::shape_t>& shapes,
                      const std::vector<tinyobj::material_t>& materials,
                      bool triangulate = true) {
  std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
            << std::endl;

  std::cout << "# of shapes    : " << shapes.size() << std::endl;
  std::cout << "# of materials : " << materials.size() << std::endl;

  for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
    printf("  v[%ld] = (%f, %f, %f)\n", v,
           static_cast<const double>(attrib.vertices[3 * v + 0]),
           static_cast<const double>(attrib.vertices[3 * v + 1]),
           static_cast<const double>(attrib.vertices[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
    printf("  n[%ld] = (%f, %f, %f)\n", v,
           static_cast<const double>(attrib.normals[3 * v + 0]),
           static_cast<const double>(attrib.normals[3 * v + 1]),
           static_cast<const double>(attrib.normals[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
    printf("  uv[%ld] = (%f, %f)\n", v,
           static_cast<const double>(attrib.texcoords[2 * v + 0]),
           static_cast<const double>(attrib.texcoords[2 * v + 1]));
  }

  for (size_t i = 0; i < shapes.size(); i++) {
    printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
    printf("Size of shape[%ld].indices: %ld\n", i,
           shapes[i].mesh.indices.size());

    if (triangulate) {
      printf("Size of shape[%ld].material_ids: %ld\n", i,
             shapes[i].mesh.material_ids.size());
      assert((shapes[i].mesh.indices.size() % 3) == 0);
      for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
        tinyobj::index_t i0 = shapes[i].mesh.indices[3 * f + 0];
        tinyobj::index_t i1 = shapes[i].mesh.indices[3 * f + 1];
        tinyobj::index_t i2 = shapes[i].mesh.indices[3 * f + 2];
        printf("  idx[%ld] = %d/%d/%d, %d/%d/%d, %d/%d/%d. mat_id = %d\n", f,
               i0.vertex_index, i0.normal_index, i0.texcoord_index,
               i1.vertex_index, i1.normal_index, i1.texcoord_index,
               i2.vertex_index, i2.normal_index, i2.texcoord_index,
               shapes[i].mesh.material_ids[f]);
      }
    } else {
      for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[f];
        printf("  idx[%ld] = %d/%d/%d\n", f, idx.vertex_index, idx.normal_index,
               idx.texcoord_index);
      }

      printf("Size of shape[%ld].material_ids: %ld\n", i,
             shapes[i].mesh.material_ids.size());
      assert(shapes[i].mesh.material_ids.size() ==
             shapes[i].mesh.num_face_vertices.size());
      for (size_t m = 0; m < shapes[i].mesh.material_ids.size(); m++) {
        printf("  material_id[%ld] = %d\n", m, shapes[i].mesh.material_ids[m]);
      }
    }

    printf("shape[%ld].num_faces: %ld\n", i,
           shapes[i].mesh.num_face_vertices.size());
    for (size_t v = 0; v < shapes[i].mesh.num_face_vertices.size(); v++) {
      printf("  num_vertices[%ld] = %ld\n", v,
             static_cast<long>(shapes[i].mesh.num_face_vertices[v]));
    }

    // printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
    // assert((shapes[i].mesh.positions.size() % 3) == 0);
    // for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
    //  printf("  v[%ld] = (%f, %f, %f)\n", v,
    //    static_cast<const double>(shapes[i].mesh.positions[3*v+0]),
    //    static_cast<const double>(shapes[i].mesh.positions[3*v+1]),
    //    static_cast<const double>(shapes[i].mesh.positions[3*v+2]));
    //}

    printf("shape[%ld].num_tags: %ld\n", i, shapes[i].mesh.tags.size());
    for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
      printf("  tag[%ld] = %s ", t, shapes[i].mesh.tags[t].name.c_str());
      printf(" ints: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
        printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
        if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" floats: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
        printf("%f", static_cast<const double>(
                         shapes[i].mesh.tags[t].floatValues[j]));
        if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" strings: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
        printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
        if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");
      printf("\n");
    }
  }

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
    printf("  material.Ka = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].ambient[0]),
           static_cast<const double>(materials[i].ambient[1]),
           static_cast<const double>(materials[i].ambient[2]));
    printf("  material.Kd = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].diffuse[0]),
           static_cast<const double>(materials[i].diffuse[1]),
           static_cast<const double>(materials[i].diffuse[2]));
    printf("  material.Ks = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].specular[0]),
           static_cast<const double>(materials[i].specular[1]),
           static_cast<const double>(materials[i].specular[2]));
    printf("  material.Tr = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].transmittance[0]),
           static_cast<const double>(materials[i].transmittance[1]),
           static_cast<const double>(materials[i].transmittance[2]));
    printf("  material.Ke = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].emission[0]),
           static_cast<const double>(materials[i].emission[1]),
           static_cast<const double>(materials[i].emission[2]));
    printf("  material.Ns = %f\n",
           static_cast<const double>(materials[i].shininess));
    printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
    printf("  material.dissolve = %f\n",
           static_cast<const double>(materials[i].dissolve));
    printf("  material.illum = %d\n", materials[i].illum);
    printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    printf("  material.map_Ns = %s\n",
           materials[i].specular_highlight_texname.c_str());
    printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
    printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
    printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
    printf("  material.refl = %s\n", materials[i].reflection_texname.c_str());
    std::map<std::string, std::string>::const_iterator it(
        materials[i].unknown_parameter.begin());
    std::map<std::string, std::string>::const_iterator itEnd(
        materials[i].unknown_parameter.end());

    for (; it != itEnd; it++) {
      printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("\n");
  }
}*/
bool Object::loadMesh(const char* basepath = NULL,
                        bool triangulate = true){
    glGenBuffers(1, &trianglesbuffer);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 7, trianglesbuffer); // Buffer Binding 7
    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; 
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizebuffer *  sizeof(triangle), &triangles[0], GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    //loadOBJ(meshFilename,vertices,uvs,normals);
    //std::cout << "size vertice " << vertices.size() << std::endl;
	  //std::cout << "finish loading" << std::endl;


    //glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(sizeof(triangle)), &triangles[0]); 
    

    std::cout << "step 2" << std::endl;
    glGenBuffers(1, &TOPtree);
  //    GLuint block_index = 0;
  //block_index = glGetProgramResourceIndex(programID_build, GL_SHADER_STORAGE_BLOCK, "TOPtree");
 // GLuint ssbo_binding_point_index = 13;
 // glShaderStorageBlockBinding(programID_build, block_index, ssbo_binding_point_index);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 13, TOPtree); // Buffer Binding 7
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizenodes * sizeof(node), &nodes[0], GL_DYNAMIC_COPY);
    //glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,  4*  112597 * (sizeof(node)), &nodes[0]);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    std::cout << "step 3" << std::endl;
    glGenBuffers(1, &rootgl);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 29, rootgl); // Buffer Binding 7
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(root), &root, GL_DYNAMIC_COPY);
    //glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(rootgl), &root); 
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    std::cout << "step 4" << std::endl; 
    std::cout << "Loading " << meshFilename << std::endl;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string error;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warnings, &error,
                                meshFilename, basepath, triangulate);

  	
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
    for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        glm::vec4 vec = glm::vec4(double(attrib.vertices[3 * v + 0]),double(attrib.vertices[3 * v + 1]),double(attrib.vertices[3 * v + 2]),1);
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
          triangle tri;
          tri.a = vert[i0.vertex_index];
          tri.b = vert[i1.vertex_index];
          tri.c = vert[i2.vertex_index];
          triangles[count] = tri;
          //std::cout << "dddd" << std::endl;
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
    
    std::cout << "size " << sizeof(triangles)/sizeof(triangle) << std::endl;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesbuffer);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &triangles, sizebuffer * sizeof(triangle));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); 

    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);
    glUnmapBuffer( GL_ARRAY_BUFFER );

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);
    glUnmapBuffer( GL_ARRAY_BUFFER );

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
     std::cout << "step 1" << std::endl;
    glUnmapBuffer( GL_ARRAY_BUFFER );
    /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, TOPtree);
    GLvoid* t = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(t, &TOPtree, (sizebuffer+1) * sizeof(node));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); */

    /*+for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        glm::vec3 vec = glm::vec3(float(attrib.vertices[3 * v + 0]),float(attrib.vertices[3 * v + 1]),float(attrib.vertices[3 * v + 2]));
        vertices.push_back(vec);
    }

    for(int i = 0;i < attrib.texcoords.size();i+=2){
        glm::vec2 vec = glm::vec2(attrib.texcoords[i],attrib.texcoords[i+1]);
        uvs.push_back(vec);
    }

    for(int i = 0;i < attrib.normals.size();i+=3){
        glm::vec3 vec = glm::vec3(float(attrib.normals[i]),float(attrib.normals[i+1]),float(attrib.normals[i+2]));
        normals.push_back(vec);
    }
    	// For each vertex of each triangle
	  for( unsigned int i=0; i<vertices.size(); i++ ){

        // Get the indices of its attributes
        unsigned int vertexIndex = vertices[i];
        unsigned int uvIndex = uvs[i];
        unsigned int normalIndex = normals[i];

        // Get the attributes thanks to the index
        glm::vec3 vertex = vertices[ vertexIndex-1 ];
        glm::vec2 uv = uvs[ uvIndex-1 ];
        glm::vec3 normal = normals[ normalIndex-1 ];

        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_uvs     .push_back(uv);
        out_normals .push_back(normal);

	    }*/
   
    
    return true;
}

void Object::draw(){
  glUseProgram(programID_build);


  //std::cout << "step 4" << std::endl;
  glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	//glm::vec4 rotatedLightPos = lightM * lightPos;
	
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1ui64NV(sizeBufferID, size);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
  //glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 13, TOPtree); // Buffer Binding 7
  //glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 29, rootgl); // Buffer Binding 7



  glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesbuffer);
  GLvoid* t = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  //std::cout << sizeof(p) << std::endl;
  memcpy(&trianglesc,t, sizebuffer * sizeof(triangle));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    std::cout << "first"<< std::endl;
        

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, TOPtree);
  GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  //std::cout << sizeof(p) << std::endl;
  memcpy(&nodes,p, sizenodes * sizeof(node));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  std::cout << "second"<< std::endl;

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootgl);
  GLvoid* r = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  memcpy(&root,r, sizeof(root));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  std::cout << "third"<< std::endl;
    
  glDispatchCompute( ceil(sizebuffer / 512.0), 1, 1);
  glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
  

  /*glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

  glUseProgram(programID_build);
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1ui64NV(sizeBufferID, size);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
  glDispatchCompute( ceil(112596 / 512.0), 1, 1);
  glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );*/
  //glBindBuffer(GL_ARRAY_BUFFER, trianglesbuffer);
  /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, TOPtree);
  GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  memcpy(p, &nodes, 112597 * 4 * sizeof(node));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootgl);
  GLvoid* r = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  memcpy(r, &root, sizeof(root));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/
  //glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

  //glBindBuffer(GL_ARRAY_BUFFER, TOPtree);

  //glBindBuffer(GL_ARRAY_BUFFER, root);
  
  //std::cout << " size top " << nodes.size() << std::endl;

  glUseProgram(programID_render);
  
  //glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 13, TOPtree); // Buffer Binding 7
  //glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 29, rootgl); // Buffer Binding 7

  //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER)
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1ui64NV(sizeBufferID, size);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
  
	glm::mat4 lrot = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(0,1,0));

	lightPos =  lrot * lightPos;
	
	static int factor= 0.0;
	lightPos.x += 0.1*factor;

	if(lightPos.x>7.0){
		factor=-1;
	}
	else if(lightPos.x<-7.0){
		factor=1;
	}
    
	//glm::mat4 lightM(1.0f);
	//lightM = glm::rotate(lightM, lightRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//lightM = glm::rotate(lightM, lightRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//lightM = glm::rotate(lightM, lightRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

	//glm::vec4 rotatedLightPos = lightM * lightPos;
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

  //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  //glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  //glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);

		// 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);
 // glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 3, vertexbuffer); // Buffer Binding 7
  //glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 4, uvbuffer); // Buffer Binding 7
  //glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 5, normalbuffer); // Buffer Binding 7

  glBindBuffer (GL_SHADER_STORAGE_BUFFER, TOPtree); // Buffer Binding 7
  //glBindBuffer(GL_ARRAY_BUFFER, TOPtree);
  //glBindBuffer (GL_ARRAY_BUFFER, root); // Buffer Binding 7
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootgl);


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootgl);
  r = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  memcpy(&root,r, sizeof(root));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


		// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  //std::cout << vertices.size() << std::endl;
 	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
    std::cout << "root" << root << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 // glBindBuffer( GL_ARRAY_BUFFER, 0 );

}
