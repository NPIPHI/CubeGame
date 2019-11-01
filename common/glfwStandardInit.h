//
// Created by 16182 on 9/16/2019.
//

#ifndef TUTORIALS_GLFWSTANDARDINIT_H
#define TUTORIALS_GLFWSTANDARDINIT_H


#include <GLFW/glfw3.h>
#include <vector>
#include "shader.hpp"

class glfwStandardInit {
public:
    static GLFWwindow* init(int width, int height, const char * name);
    static void terminate(const std::vector<shader> shaders);
};


#endif //TUTORIALS_GLFWSTANDARDINIT_H
