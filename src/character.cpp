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
    glm::vec2 delta = viewCamera.getXZtransform() * input.getJoystick();
    position.x += delta.x;
    position.z += delta.y;
    position.y += (input.getJump() - input.getShift());

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
