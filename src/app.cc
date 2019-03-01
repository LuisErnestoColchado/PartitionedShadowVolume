#include "headers/app.h"
#include <new>
#include <new>
app::app(std::vector<object*> objs,
        int sizeTriangles){

    this->objects = objs;
    this->sizeTriangles = sizeTriangles;

    this->lightPos = glm::vec4(0,15,5,0);
    this->colores[0] = glm::vec3(0.7,0.7,0.7);
    this->colores[1] = glm::vec3(0.2,0.2,0.2);

    this->sizeNodes = (sizeTriangles * 4) + 1 ;

    /*  Dynamic arrays __Errors__*/
    //this->triangles = (triangle*)malloc(this->sizeTriangles*sizeof(triangle));
    //this->nodes = (node*)malloc(this->sizeNodes*sizeof(node));
    //this->triangles = new (std::nothrow) triangle[sizeTriangles];
    //this->nodes = new (std::nothrow) node[sizeNodes];
    //trianglesc = new triangle[sizeTriangles];
}

void app::getTriangles(){

    glGenBuffers(1, &trianglesBuffer);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 7, trianglesBuffer);
    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeTriangles *  sizeof(triangle), &triangles[0], GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER);

    int count = 0;
    for (auto obj : objects){
        for(int i = 0;i < obj->vertices.size();i+=3){
            if(obj->generateShadow == true){
                triangle tri;
                tri.a = obj->vertices[i];
                tri.b = obj->vertices[i+1];
                tri.c = obj->vertices[i+2];
                triangles[count] = tri;
                count+=1;
            }
        }
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesBuffer);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &triangles, sizeTriangles * sizeof(triangle));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}

void app::setShadersBuild(const char * sFile){
    programBuild = LoadShadersBuild(sFile);
    lightID = glGetUniformLocation(programBuild, "LightPosition_worldspace");
}

void app::buildingTOPtree(){

    variable.node = 0;
    variable.triangle = 0;
    glGenBuffers(1, &TOPTREE);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 13, TOPTREE);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeNodes * sizeof(node), &nodes[0], GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    glGenBuffers(1, &rootGL);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 29, rootGL);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(root), &root, GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    glGenBuffers(1, &utilGL);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 30, utilGL);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(util), &variable, GL_DYNAMIC_COPY);
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    glm::mat4 lrot = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(0,1,0));
    lightPos =  lrot * lightPos;
    int factor = 1;
    lightPos.x += 0.01*factor;
    lightPos.y += 0.01*factor;

    glUseProgram(programBuild);
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
    glDispatchCompute( ceil(sizeTriangles / 36.0), 1, 1);
    glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

   /*
    * For debug
    *
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, trianglesBuffer);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(&trianglesc,p, sizeTriangles * sizeof(triangle));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    std::cout << "triangle p2 " << trianglesc[2].a[2] << std::endl;*/

    /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootGL);
    GLvoid* r = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(&root,r, sizeof(root));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    std::cout << "root " << root << std::endl;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, TOPTREE);
    GLvoid* t = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(&nodes,t, sizeNodes * sizeof(node));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    std::cout << "node " << nodes[2].plane[1] << std::endl;
    *
    *
    */
}

void app::rendering(){

    int count = 0;
    glm::mat4 lrot = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(0,1,0));
    lightPos =  lrot * lightPos;
    for(auto object : objects){
        glUseProgram(object->programRender);


        glm::mat4 ProjectionMatrix = getProjectionMatrix();
	      object->viewMatrix = getViewMatrix();
	      object->MVP = ProjectionMatrix * object->viewMatrix * object->modelMatrix;

        glUniformMatrix4fv(object->matrixID, 1, GL_FALSE, &(object->MVP[0][0]));
        glUniformMatrix4fv(object->modelMatrixID, 1, GL_FALSE, &(object->modelMatrix[0][0]));
        glUniformMatrix4fv(object->viewMatrixID, 1, GL_FALSE, &(object->viewMatrix[0][0]));

        glUniform3f(object->lightID, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(object->colorID,  colores[count].x, colores[count].y, colores[count].z);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object->texture);

        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(object->textureID, 0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, object->vertexBuffer);

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
        glBindBuffer(GL_ARRAY_BUFFER, object->uvBuffer);
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
        glBindBuffer(GL_ARRAY_BUFFER, object->normalBuffer);
        glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );

        glBindBuffer (GL_SHADER_STORAGE_BUFFER, TOPTREE);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootGL);

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, object->vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        count++;
    }
}

void app::cleanBuffers(){
    root = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootGL);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &root, sizeof(root));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


}
