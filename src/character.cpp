//
// Created by 16182 on 10/30/2019.
//

#include "character.h"
#include "geometry.h"
#include <iostream>
#include <GLM/vec3.hpp>
#include <GLM/mat2x2.hpp>

void character::update(GameMap &worldMap) {
    input.poll(window);
    viewCamera.updateViewFromMouse(input.getMouseX(), input.getMouseY());
    glm::vec2 XZdelta = viewCamera.getXZtransform() * input.getJoystick();
    glm::vec3 delta = glm::vec3(XZdelta.x, (input.getJump() - input.getShift()), XZdelta.y);
    delta *= 0.1;
    position += delta;
    collisionTest(worldMap);
    viewCamera.setPosition(position);
    if (input.getLeftToggle()) {
        breakBlock(worldMap);
    }
    if (input.getRightToggle()) {
        placeBlock(1, worldMap);
    }
}

character::character(GLFWwindow *window) {
    this->window = window;
    position = glm::vec3(10, 20, 10);
    viewCamera = AxisCamera(1024.f / 720.f);
    viewCamera.setPosition(position);
    viewCamera.setView(window, {15, 18, 10});
}

void character::placeBlock(char blockID, GameMap &worldMap) {
    glm::vec3 selectedBlock = viewCamera.rayCastToPreviousBlock(10, worldMap);
    worldMap.changeBlock(selectedBlock, blockID);
}

void character::breakBlock(GameMap &worldMap) {
    glm::vec3 selectedBlock = viewCamera.rayCastToBlock(10, worldMap);
    worldMap.changeBlock(selectedBlock, 0);
}

glm::mat4 character::getView() const {
    return viewCamera.getView();
}

glm::vec3 character::getSelectedBlock(const GameMap &worldMap) {
    return viewCamera.rayCastToBlock(10, worldMap);
}

void character::collisionTest(const GameMap &worldMap) {
    std::vector<axisCube> possibleCubes;
    axisCube collisionBox = axisCube(position - glm::vec3(0.3, 1.5, 0.3), position + glm::vec3(0.3, 0.5, 0.3));
    for (int x = -1; x <= 1; x++) {
        for (int y = -2; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                if (worldMap.valueAt(intVect(x, y, z) + position)) {
                    possibleCubes.push_back(unitCube(intVect(x, y, z) + position));
                }
            }
        }
    }
    for (const axisCube &cube : possibleCubes) {
        ejectData collision = collisionBox.intersect(cube);
        if (collision.intersects && !collision.corner) {
            position += collision.normal * collision.distance;
            collisionBox += collision.normal * collision.distance;
        }
    }
}
