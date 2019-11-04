//
// Created by 16182 on 10/19/2019.
//

#include <glm/gtc/matrix_transform.hpp>
#include "AxisCamera.h"
#define MOUSESENSITVITY 0.005

glm::vec3 intCast(glm::vec3 vector){
    return glm::vec3(int(vector.x), int(vector.y), int(vector.z));
}

AxisCamera::AxisCamera(float aspectRatio) : camera(45, aspectRatio, 0.1f, 100.f) {
    xRot = 0;
    yRot = 0;
    mousePosX = 500;
    mousePosY = 500;
    mouseRefrenceX = 500;
    mouseRefrenceY = 500;
}

void AxisCamera::control(GLFWwindow *window) {
    glfwGetCursorPos(window, &mousePosX, &mousePosY);
    updateViewFromMouse(mousePosX, mousePosY);

    glm::vec4 deltaVect;
    if (glfwGetKey(window, GLFW_KEY_A)) {
        deltaVect.x -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        deltaVect.x += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        deltaVect.z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        deltaVect.z += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
        deltaVect.y += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
        deltaVect.y -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_X)) {
        deltaVect *= 10;
    }
    position += glm::vec3(deltaVect * glm::rotate(glm::mat4(), float(xRot), glm::vec3(0, 1, 0)));
    setPosition(position);
}

void AxisCamera::setView(GLFWwindow *window, glm::vec3 target) {
    glm::vec3 normalizedDiffrence = position - target;
    normalizedDiffrence /= glm::length(normalizedDiffrence);
    if(normalizedDiffrence.x == 0){
        xRot = 0;
    } else {
        xRot = glm::atan(normalizedDiffrence.z / normalizedDiffrence.x) - pi/2 + std::signbit(normalizedDiffrence.x) * pi;
    }
    yRot = -glm::asin(normalizedDiffrence.y);

    glfwGetCursorPos(window, &mousePosX, &mousePosY);
    mouseRefrenceX = mousePosX - xRot / MOUSESENSITVITY;
    mouseRefrenceY = mousePosY + yRot / MOUSESENSITVITY;
    setRotation(glm::vec3(0, 1, 0), xRot);
    rotate(glm::vec3(glm::cos(xRot+pi), 0, glm::sin(xRot+pi)), yRot);
}

glm::vec3 AxisCamera::rayCast(double distance) {
    return  forewardVector() + position;
}

std::vector<glm::vec3> AxisCamera::rayCastBlocks(double maxDistance) {//poor way to do this, temporary
    std::vector<glm::vec3> retVect{};
    retVect.push_back(intCast(position));
    glm::vec3 delta = forewardVector();
    for(float i = 0; i < maxDistance*10; i++){
        glm::vec3 addVect = position + forewardVector() * (i/10);
        if(!(retVect.back() == intCast(addVect))){
            retVect.push_back(intCast(addVect));
        }
    }
    return retVect;
}

glm::vec3 AxisCamera::forewardVector() {
    return glm::vec3(glm::rotate(glm::rotate(glm::mat4(1), float(-yRot), glm::vec3(glm::cos(xRot+pi), 0, glm::sin(xRot+pi))), float(-xRot+pi/2), glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 0));
}

glm::vec3 AxisCamera::rayCastToBlock(double maxDistance, const GameMap &refMap) {
    auto checkBlocks = rayCastBlocks(maxDistance);
    for (const auto &block : checkBlocks) {
        if(refMap.valueAt(block.x, block.y, block.z)) {
            return block;
        }
    }
    return glm::vec3(0, 0, 0);
}

glm::vec3 AxisCamera::rayCastToPreviousBlock(double maxDistance, const GameMap &refMap) {
    auto checkBlocks = rayCastBlocks(maxDistance);
    for (int i = 0; i < checkBlocks.size(); i++){
        if(refMap.valueAt(checkBlocks[i].x, checkBlocks[i].y, checkBlocks[i].z)){
            if(i == 0) {
                return checkBlocks[i];
            } else {
                return checkBlocks[i-1];
            }
        }
    }
    return glm::vec3(0, 0, 0);
}

AxisCamera::AxisCamera() : camera(45, 16.f / 9.f, 0.1f, 100.f) {
    xRot = 0;
    yRot = 0;
    mousePosX = 500;
    mousePosY = 500;
    mouseRefrenceX = 500;
    mouseRefrenceY = 500;
}

void AxisCamera::updateViewFromMouse(double mouseX, double mouseY) {
    mousePosX = mouseX;
    mousePosY = mouseY;
    xRot = (mousePosX - mouseRefrenceX) * MOUSESENSITVITY;
    yRot = -(mousePosY - mouseRefrenceY) * MOUSESENSITVITY;

    if (yRot < -pi / 2) {
        yRot = -pi / 2;
        mouseRefrenceY = mousePosY - pi / (2 * MOUSESENSITVITY);
    }

    if (yRot > pi / 2) {
        yRot = pi / 2;
        mouseRefrenceY = mousePosY + pi / (2 * MOUSESENSITVITY);
    }
    setRotation(glm::vec3(0, 1, 0), xRot);
    rotate(glm::vec3(glm::cos(xRot + pi), 0, glm::sin(xRot + pi)), yRot);
}

glm::mat2 AxisCamera::getXZtransform() {
    return glm::mat2(-glm::cos(xRot + pi / 2), -glm::sin(xRot + pi / 2), -glm::sin(xRot + pi / 2),
                     glm::cos(xRot + pi / 2));//looks messy, but is correct
}
