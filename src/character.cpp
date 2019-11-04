//
// Created by 16182 on 10/30/2019.
//

#include "character.h"
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
    viewCamera = AxisCamera(1024.f / 720.f);
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
