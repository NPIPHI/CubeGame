//
// Created by 16182 on 11/1/2019.
//

#ifndef MINECROFT_CONTROLLER_H
#define MINECROFT_CONTROLLER_H


#include <GLFW/glfw3.h>
#include <GLM/vec2.hpp>

class controller;
static controller* currentController;

class controller {
public:
    void poll(GLFWwindow *window);//updates mouse toggle state
    controller();
    glm::vec2 getJoystick() const;
    bool getJump() const;
    bool getShift() const;
    bool getLeftToggle() const;
    bool getRightToggle() const;
    bool getLeftClick() const;
    bool getRightClick() const;
    double getMouseX() const;
    double getMouseY() const;
private:
    bool forward, left, right, back, jump, shift, leftClick, rightClick, leftToggle, rightToggle;
    double mouseX, mouseY;
    //static void keyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods);
    //static void mousePosCallback(GLFWwindow* window, double xPos, double yPos);
    //static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};


#endif //MINECROFT_CONTROLLER_H
