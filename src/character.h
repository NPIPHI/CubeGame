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
    explicit character(GLFWwindow *window);

    AxisCamera viewCamera;

    void update(GameMap &worldMap);

    glm::mat4 getView() const;

    glm::vec3 getSelectedBlock(const GameMap &worldMap);
private:
    void placeBlock(char blockID, GameMap &worldMap);

    void collisionTest(const GameMap &worldMap);
    void breakBlock(GameMap &worldMap);

    GLFWwindow *window;
    controller input;
    glm::vec3 position;
};


#endif //MINECROFT_CHARACTER_H
