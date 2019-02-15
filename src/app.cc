#include "headers/app.h"
app::app(const char * compute,const char * fragment,std::vector<object*> objs){
    glGenVertexArrays(1, &VertexArrayID);
    computeFile = compute;
    fragmentFile = fragment;
    objects = objs;
}
void app::setShadersBuild(){
    programBuild = LoadShadersBuild(computeFile);

    matrixID = glGetUniformLocation(programBuild, "MVP");
    viewMatrixID = glGetUniformLocation(programBuild, "V");
    modelMatrixID = glGetUniformLocation(programBuild, "M");
    textureID  = glGetUniformLocation(programBuild, "myTextureSampler");
    lightID = glGetUniformLocation(programBuild, "LightPosition_worldspace");
    sizeBufferID = glGetUniformLocation(programBuild,"sizeBuffer");

    glGenBuffers(1, &trianglesBuffer);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 7, trianglesBuffer);
    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeTriangles *  sizeof(triangle), &triangles[0], GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    glGenBuffers(1, &TOPtree);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 13, TOPtree);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeNodes * sizeof(node), &nodes[0], GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    glGenBuffers(1, &rootGL);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 29, rootGL);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(root), &root, GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    int count = 0;

    for(auto o : objects)
        sizeTriangles += o->vertices.size() / 3;

    triangles = new triangle[sizeTriangles];
    nodes = new node[sizeTriangles * 4 + 1];

    for (auto obj : objects){
        for(int i = 0;i < obj->vertices.size();i+=3){
            totalVertices.push_back(obj->vertices[i]);
            totalVertices.push_back(obj->vertices[i+1]);
            totalVertices.push_back(obj->vertices[i+2]);
            triangle tri;
            tri.a = obj->vertices[i];
            tri.b = obj->vertices[i+1];
            tri.c = obj->vertices[i+2];
            triangles[count] = tri;
            count+=1;
        }
        std::cout << "ddddd" << std::endl;
        for(int i = 0;i < obj->uvs.size();i+=3){
            totalUvs.push_back(obj->uvs[i]);
            totalUvs.push_back(obj->uvs[i+1]);
            totalUvs.push_back(obj->uvs[i+2]);
        }
        for(int i = 0;i < obj->normals.size();i+=3){
            totalNormals.push_back(obj->normals[i]);
            totalNormals.push_back(obj->normals[i+1]);
            totalNormals.push_back(obj->normals[i+2]);
        }
    }

}

void app::setShadersRender(){
    programFragment = LoadShadersBuild(fragmentFile);

    matrixID = glGetUniformLocation(programFragment, "MVP");
    viewMatrixID = glGetUniformLocation(programFragment, "V");
    modelMatrixID = glGetUniformLocation(programFragment, "M");
    textureID  = glGetUniformLocation(programFragment, "myTextureSampler");
    lightID = glGetUniformLocation(programFragment, "LightPosition_worldspace");
    sizeBufferID = glGetUniformLocation(programFragment,"sizeBuffer");
}

void app::buildingTOPtree(){

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesBuffer);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &triangles, sizeTriangles * sizeof(triangle));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, totalVertices.size() * sizeof(glm::vec4), &totalVertices[0], GL_STATIC_DRAW);
    glUnmapBuffer( GL_ARRAY_BUFFER );

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, totalUvs.size() * sizeof(glm::vec3), &totalUvs[0], GL_STATIC_DRAW);
    glUnmapBuffer( GL_ARRAY_BUFFER );

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, totalNormals.size() * sizeof(glm::vec3), &totalNormals[0], GL_STATIC_DRAW);
    std::cout << "step 1" << std::endl;
    glUnmapBuffer( GL_ARRAY_BUFFER );

    glm::mat4 lrot = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(0,1,0));

  	lightPos =  lrot * lightPos;

      //change light
  	//if(time % 200 == 0)
      //lightPos.x += 0.1*factor;

  	if(lightPos.x>7.0){
  		factor=-1;
  	}
  	else if(lightPos.x<-7.0){
  		factor=1;
  	}

    std::cout << "Starting the build of the TOP TREE" << std::endl;

    glUseProgram(programBuild);

    glm::mat4 projectionMatrix = getProjectionMatrix();
    viewMatrix = getViewMatrix();
    MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1ui64NV(sizeBufferID, sizeTriangles);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

    glDispatchCompute( ceil(sizeTriangles / 512.0), 1, 1);
    glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

}

void app::rendering(){
    std::cout << "Starting rendering" << std::endl;
    glUseProgram(programFragment);
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1ui64NV(sizeBufferID, sizeTriangles);
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(textureID, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
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
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
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
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

    glBindBuffer (GL_SHADER_STORAGE_BUFFER, TOPtree);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootGL);

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, totalVertices.size() );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
