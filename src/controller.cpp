//
// Created by 16182 on 11/1/2019.
//

#include "controller.h"


/*void controller::attach(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyCallbackFunction);
    glfwSetCursorPosCallback(window, controller::mousePosCallback);
    glfwSetMouseButtonCallback(window, controller::mouseButtonCallback)
    currentController = this;
}

void controller::keyCallbackFunction(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_W){
            currentController->forward = true;
        }
        if(key == GLFW_KEY_A){
            currentController->left = true;
        }
        if(key == GLFW_KEY_S){
            currentController->back = true;
        }
        if(key == GLFW_KEY_D){
           currentController->right = true;
        }
        if(key == GLFW_KEY_SPACE) {
            currentController->jump = true;
        }
    }

    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_W){
            currentController->forward = false;
        }
        if(key == GLFW_KEY_A){
            currentController->left = false;
        }
        if(key == GLFW_KEY_S){
            currentController->back = false;
        }
        if(key == GLFW_KEY_D){
            currentController->right = false;
        }
        if(key == GLFW_KEY_SPACE) {
            currentController->jump = false;
        }
    }
}

void controller::mousePosCallback(GLFWwindow *window, double xPos, double yPos) {
    currentController->mouseX = xPos;
    currentController->mouseY = yPos;
}

void controller::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if(action == GLFW_PRESS){
        if(button == GLFW_MOUSE_BUTTON_LEFT){
            currentController->leftClick = true;
        }
        if(button == GLFW_MOUSE_BUTTON_RIGHT){
            currentController->rightClick = true;
        }
    }
    if(action == GLFW_RELEASE){
        if(button == GLFW_MOUSE_BUTTON_LEFT){
            currentController->leftClick = false;
        }
        if(button == GLFW_MOUSE_BUTTON_RIGHT){
            currentController->rightClick = false;
        }
    }
}*/

void controller::poll(const GLFWwindow *window) {
    leftToggle = !leftClick && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    rightToggle = !rightClick && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    leftClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    rightClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    forward = glfwGetKey(window, GLFW_KEY_W);
    back = glfwGetKey(window, GLFW_KEY_S);
    left = glfwGetKey(window, GLFW_KEY_A);
    right = glfwGetKey(window, GLFW_KEY_D);
    jump = glfwGetKey(window, GLFW_KEY_SPACE);
}

bool controller::getLeftToggle() const {
    return leftToggle;
}

bool controller::getRightToggle() const {
    return rightToggle;
}

glm::vec2 controller::getJoystick() const {
    return glm::vec2(right-left, forward-back);
}

bool controller::getJump() const {
    return jump;
}

bool controller::getRightClick() const {
    return rightClick;
}

bool controller::getLeftClick() const {
    return leftClick;
}
