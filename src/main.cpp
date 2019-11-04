//
// Created by 16182 on 10/13/2019.
//

#include <GL/glew.h>

#include <iostream>
#include "../common/glfwStandardInit.h"
#include "../common/shader.hpp"
#include <windows.h>
#include <chrono>
#include "AxisCamera.h"
#include "Chunck.h"
#include "GameMap.h"
#include "character.h"

void draw(const shader &shader, GLuint vertexBuffer, GLuint uvBuffer, int length);

void drawIndexed(const shader &shader, GLuint vertexBuffer, GLuint uvBuffer, GLuint indexBuffer, int length);

void drawLineIndexed(const shader &shader, GLuint vertexBuffer, GLuint indexBuffer, int length);

void mouseCallBack(GLFWwindow *window, int button, int action, int mods);

GameMap *mainMap;
AxisCamera *camera;

int main(){
    GLFWwindow* window = glfwStandardInit::init(1024, 720, "Not Mine Craft");

    shader vertexShader("../shaders/triVertex.glsl", "../shaders/textureFragment.glsl", {"VIEW"});
    shader wireFrameShader("../shaders/lineVertex.glsl", "../shaders/colorFragment.glsl", {"VIEW", "OFFSET"});
    vertexShader.setUniformMatrix4("VIEW", glm::mat4(1));
    wireFrameShader.setUniformMatrix4("VIEW", glm::mat4(1));

    GLfloat axisVertexData[] = {
            0, 0, 0,
            10, 0.1, 0,
            0, 0, 0.1,
            0, 0, 0,
            0.1, 10, 0,
            0.1, 0, 0,
            0, 0, 0,
            0, 0.1, 10,
            0.1, 0, 0
    };

    GLfloat uvData[] = {
            1, 0,
            1, 0,
            1, 0,
            0, 1,
            0, 1,
            0, 1,
            1, 1,
            1, 1,
            1, 1
    };

    GLfloat wireFrameData[] = {
            0, 0, 0,
            1, 0, 0,
            1, 0, 1,
            0, 0, 1,
            0, 1, 0,
            1, 1, 0,
            1, 1, 1,
            0, 1, 1
    };

    GLuint wireFrameVerticesIndexes[] = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            0, 4,
            1, 5,
            2, 6,
            3, 7,
            4, 5,
            5, 6,
            6, 7,
            7, 4
    };

    GLuint wireFrameVerts;
    glGenBuffers(1, &wireFrameVerts);
    glBindBuffer(GL_ARRAY_BUFFER, wireFrameVerts);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wireFrameData), wireFrameData, GL_STATIC_DRAW);

    GLuint wireFrameIndexes;
    glGenBuffers(1, &wireFrameIndexes);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wireFrameIndexes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wireFrameVerticesIndexes), wireFrameVerticesIndexes, GL_STATIC_DRAW);

    GLuint axisRefID;
    glGenBuffers(1, &axisRefID);
    glBindBuffer(GL_ARRAY_BUFFER, axisRefID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertexData), axisVertexData, GL_STATIC_DRAW);

    GLuint uvRefID;
    glGenBuffers(1, &uvRefID);
    glBindBuffer(GL_ARRAY_BUFFER, uvRefID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvData), uvData, GL_STATIC_DRAW);

    GLuint vertexID;
    glGenBuffers(1, &vertexID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexID);
    glBufferData(GL_ARRAY_BUFFER, Chunck::chunckIndexCount * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);

    GLuint indexID;
    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Chunck::chunckIndexCount * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

    GLuint uvID;
    glGenBuffers(1, &uvID);
    glBindBuffer(GL_ARRAY_BUFFER, uvID);
    glBufferData(GL_ARRAY_BUFFER, Chunck::chunckIndexCount * sizeof(glm::vec2), nullptr, GL_STATIC_DRAW); //must be initilized

    glfwSetMouseButtonCallback(window, mouseCallBack);

    AxisCamera gameCam(1024.f/720.f);
    camera = &gameCam;
    gameCam.setPosition({0, 0, 5});
    gameCam.setView(window, {0, 0, 0});

    GameMap world = GameMap(vertexID, uvID, indexID);
    mainMap = &world;

    character player(window);

    double totalFrameTime = 0;
    int trackedFrames = 0;

    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_R)) {
            gameCam.setView(window, {0, 0, 0});
        }
        //gameCam.control(window);
        player.update(world);
        vertexShader.setUniformMatrix4("VIEW", player.getView());
        wireFrameShader.setUniformMatrix4("VIEW", player.getView());

        if(!glfwGetKey(window, GLFW_KEY_F)){

            drawIndexed(vertexShader, vertexID, uvID, indexID, world.getVertexCount());
            auto selectedBlock = player.getSelectedBlock(world); // bad name
            wireFrameShader.setUniformVec3("OFFSET", selectedBlock);
            drawLineIndexed(wireFrameShader, wireFrameVerts, wireFrameIndexes, 24);

        } else {
            auto timeBefore = std::chrono::system_clock::now();
            draw(vertexShader, axisRefID, uvRefID, 9);
            //drawIndexed(vertexShader, vertexID, uvID, indexID, Chunck::chunckIndexCount);
            drawIndexed(vertexShader, vertexID, uvID, indexID, world.getVertexCount());
            glFinish();
            auto timeAfter = std::chrono::system_clock::now();
            std::cout << "Frame Time: " << (timeAfter - timeBefore).count()/1000 << "ns" << std::endl;
            totalFrameTime += (timeAfter - timeBefore).count();
            trackedFrames++;
            std::cout << "Avg Frame Time: " << totalFrameTime/trackedFrames/1000 << "ns" << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(16);
    } while(!glfwGetKey(window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(window));

    glfwStandardInit::terminate({vertexShader});
}

void draw(const shader &shader, GLuint vertexBuffer, GLuint uvBuffer, int length){
    glUseProgram(shader.shaderID);
    shader.passUniforms();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
            );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );


    glDrawArrays(GL_TRIANGLES, 0, length);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void drawIndexed(const shader &shader, GLuint vertexBuffer, GLuint uvBuffer, GLuint indexBuffer, int length){
    glUseProgram(shader.shaderID);
    shader.passUniforms();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void drawLineIndexed(const shader &shader, GLuint vertexBuffer, GLuint indexBuffer, int length){
    glUseProgram(shader.shaderID);
    shader.passUniforms();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_LINES, length, GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(0);
}

void mouseCallBack(GLFWwindow *window, int button, int action, int mods) {//TODO make part of player
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == 1) {
            glm::vec3 selectedBlock = camera->rayCastToBlock(10, *mainMap); // bad name
            mainMap->changeBlock(selectedBlock.x, selectedBlock.y, selectedBlock.z, 0);
        }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(action == 1) {
            glm::vec3 selectedBlock = camera->rayCastToPreviousBlock(10, *mainMap); // bad name
            mainMap->changeBlock(selectedBlock.x, selectedBlock.y, selectedBlock.z, 1);
        }
    }
}