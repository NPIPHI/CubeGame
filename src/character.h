//
// Created by 16182 on 10/30/2019.
//

#ifndef MINECROFT_CHARACTER_H
#define MINECROFT_CHARACTER_H


#include <glm/vec3.hpp>
#include "geometry.h"
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

    void applyPhysics();
    void breakBlock(GameMap &worldMap);

    bool grounded, walking;
    GLFWwindow *window;
    controller input;
    glm::vec3 position;
    glm::vec3 velocity;
    axisCube hitBox;
    const float mass = 60;
    const float groundFriction = 5; //idk, larger to add snappiness
    const float airResistanceConstant = 3; //idk
    const float gravity = 9.81; //m/ss
    const float frameRate = 30; //fps
    const float legForce = 840; //watts
    const float walkingFrictionFraction = 0.05;
    const float staticFrictonThreashold = 0.3;
};


#endif //MINECROFT_CHARACTER_H
