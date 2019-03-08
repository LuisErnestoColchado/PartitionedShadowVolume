#include "headers/app.h"
#include <new>
#include <chrono>
#include <ctime>
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;

app::app(std::vector<object*> objs,
        int sizeTriangles){

    this->objects = objs;
    this->sizeTriangles = sizeTriangles;

//OK 0,15,5
    this->lightPos = glm::vec4(1, 15, 2,0);
    this->colores[0] = glm::vec3(0.7,0.7,0.7);
    this->colores[1] = glm::vec3(0.18,0.18,0.18);

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

    //auto startB = Time::now();
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
}

void app::setShadersBuild(const char * sFile){
    programBuild = LoadShadersBuild(sFile);
    lightID = glGetUniformLocation(programBuild, "LightPosition_worldspace");
}

void app::buildingTOPtree(){

    cleanBuffers();
    auto startB = Time::now();

    glm::mat4 lrot = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(0,1,0));
    lightPos =  lrot * lightPos;
    int factor = 1;
    //lightPos.x -= 0.01*factor;
    lightPos.y += 2*factor;

    glUseProgram(programBuild);
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
    glDispatchCompute( ceil(sizeTriangles / 512.0), 1, 1);
    glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
    glFinish();
    auto endB = Time::now();
    fsec elapsed_secondsB = endB - startB;


    //std::cout<<"Building Top Tree Time: "<< elapsed_secondsB.count() * 1000.0 <<'\n';
    buildingTime += elapsed_secondsB.count() * 1000.0;
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

    auto startR = Time::now();
    //std::clock_t startR;
    //double durationR;
    //startR = std::clock();
    int count = 0;
    /*glm::mat4 lrot = glm::rotate(glm::mat4(1.0),0.0f,glm::vec3(0,1,0));
    lightPos =  lrot * lightPos;
    int factor = 1;
    lightPos.x += 0.001*factor;
    lightPos.y += 0.001*factor;*/
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

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, object->vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        count++;
    }
    glFinish();
    auto endR = Time::now();
    fsec elapsed_secondsR = endR - startR;

    //std::cout<<"Rendering Time : "<< elapsed_secondsR.count() * 1000.0 << '\n';
    fragmentTime += elapsed_secondsR.count() * 1000.0;
}

void app::cleanBuffers(){
    root = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rootGL);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &root, sizeof(root));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    for(int i = 0; i < sizeNodes;i++){
        glm::vec4 vec = glm::vec4(0);
        uint array[4];
        nodes[i].plane = vec;
                nodes[i].link[0] = 0;   nodes[i].link[1] = 0;   nodes[i].link[2] = 0;   nodes[i].link[3] = 0;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, TOPTREE);
    GLvoid* t = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(t, &nodes, this->sizeNodes *sizeof(node));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    variable.node = 0;
    variable.triangle = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, utilGL);
    GLvoid* v = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(v, &variable, sizeof(variable));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}

int app::getBestTriangle(std::vector<triangle> pTriangles){
    if(pTriangles.size() > 0){
        int max = 0;
        int maxIndex = 0;
        std::vector<triangle> bestFront;

        std::vector<triangle> bestBack;

        std::vector<triangle> bestIntersect;

        for(int i = 0; i < pTriangles.size(); i++){
            auto tri = pTriangles[i];
            glm::vec3 A = tri.a - lightPos;
            glm::vec3 B = tri.b - lightPos;
            glm::vec3 C = tri.c - lightPos;

            float valuePlane = 0;

            float countFront = 0;

            float countBack = 0;

            float countIntersect = 0;

            float score = 0;

            std::vector<triangle> front;
            std::vector<triangle> back;
            std::vector<triangle> intersect;

            const glm::vec3 norm = normalize( cross(C-A, B-A) );
            const glm::vec4 cappingPlane = glm::vec4( norm, -dot(norm,A) );

            node nodePlane;
            nodePlane.plane = computePlane(A,B);

            for(int j = 0;j < pTriangles.size();j++){
                if(i != j){
                    glm::vec3 _A = pTriangles[j].a - lightPos;
                    glm::vec3 _B = pTriangles[j].b - lightPos;
                    glm::vec3 _C = pTriangles[j].c - lightPos;

                    valuePlane = calculateDistance(_A,_B,_C,nodePlane.plane);

                    if(valuePlane > 0.0){
                        countFront++;
                        front.push_back(pTriangles[j]);
                    }
                    else if(valuePlane < 0.0){
                        countBack++;
                        back.push_back(pTriangles[j]);
                    }
                    else{
                        countIntersect++;
                        intersect.push_back(pTriangles[j]);
                    }
                }
            }

            score = ALPHA * abs(countFront - countBack) + BETA * countIntersect;
            if(max < score){
                max = score;
                maxIndex = i;
                bestFront = front;
                bestBack = back;
                bestIntersect = intersect;

            }
        }
        triangles[indexOptimizeTriangles] = pTriangles[maxIndex];
        indexOptimizeTriangles++;
        return getBestTriangle(bestFront) + getBestTriangle(bestBack) + getBestTriangle(bestIntersect);
    }
    else{
        return 0;
    }

}


glm::vec4 app::computePlane(glm::vec3 v1,glm::vec3 v2)
{
		if ( v1.x < v2.x )
				return glm::vec4(  normalize( cross(v1, v2-v1) ), 0.0);
		else
				return glm::vec4( -normalize( cross(v2, v1-v2) ), 0.0);
}

int app::calculateDistance(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec4 plane)
{
		const int sig = int(glm::sign( dot(plane, glm::vec4(A, 1)) )) +
						int(glm::sign( dot(plane, glm::vec4(B, 1)) )) +
						int(glm::sign( dot(plane, glm::vec4(C, 1)) )) ;

		return abs(sig)==3 ? sig : 0;
}

void app::improvePSV(){
    auto startI = Time::now();
    std::vector<triangle> vectorTriangles(std::begin(triangles), std::end(triangles));
    getBestTriangle(vectorTriangles);
    auto endI = Time::now();
    fsec elapsed_secondsI = endI - startI;
    timeImprovePSV += elapsed_secondsI.count() * 1000.0;
}
