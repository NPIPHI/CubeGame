//
// Created by 16182 on 10/30/2019.
//

#ifndef MINECROFT_CHARACTER_H
#define MINECROFT_CHARACTER_H


#include <glm/vec3.hpp>
#include "AxisCamera.h"
#include "GameMap.h"
#include "controller.h"

class character {
public:
    explicit character(const GLFWwindow *window) : window(window);
    AxisCamera camera;
    void update(const GameMap& worldMMap);
private:
    const GLFWwindow *window;
    controller input;
    glm::vec3 position;
};


#endif //MINECROFT_CHARACTER_H