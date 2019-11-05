//
// Created by 16182 on 10/30/2019.
//

#include "character.h"
#include "geometry.h"
#include <iostream>
#include <GLM/vec3.hpp>
#include <GLM/mat2x2.hpp>
#include <glm/glm.hpp>

character::character(GLFWwindow *window) {
    this->window = window;
    position = glm::vec3(10, 40, 10);
    velocity = glm::vec3(0);
    grounded = false;
    walking = false;
    input = controller();
    viewCamera = AxisCamera(1024.f / 720.f);
    viewCamera.setPosition(position);
    viewCamera.setView(window, position + glm::vec3(5, -1, 0));
}

void character::update(GameMap &worldMap) {
    input.poll(window);
    viewCamera.updateViewFromMouse(input.getMouseX(), input.getMouseY());

    glm::vec2 XZdelta = viewCamera.getXZtransform() * input.getJoystick();
    glm::vec3 acceleration = glm::vec3(XZdelta.x, 0, XZdelta.y);
    walking = glm::length(acceleration) > 0.1;
    acceleration *= legForce / (frameRate * mass);
    velocity += acceleration;
    applyPhysics();
    position += (velocity / frameRate);

    grounded = false;
    collisionTest(worldMap);

    viewCamera.setPosition(position);
    if (input.getLeftToggle()) {
        breakBlock(worldMap);
    }
    if (input.getRightToggle()) {
        placeBlock(1, worldMap);
    }
    if (input.getJump() && grounded) {
        velocity.y += 5;
        grounded = false;
    }
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
            velocity -= glm::dot(velocity, collision.normal) * collision.normal;
            if (collision.normal.y > 0.5) {
                grounded = true;
            }
        }
    }
}

void character::applyPhysics() {
    glm::vec3 airResistance, groundResistance, gravityForce;
    float normalForce = (gravity * mass);
    gravityForce = glm::vec3(0, -gravity * mass, 0);
    airResistance = -glm::normalize(velocity) * glm::dot(velocity, velocity) * airResistanceConstant;
    if (grounded) {
        if (glm::length(glm::vec2(velocity.x, velocity.z)) > staticFrictonThreashold) {
            if (walking) {
                groundResistance =
                        -glm::normalize(glm::vec3(velocity.x, 0, velocity.z)) * groundFriction * normalForce *
                        walkingFrictionFraction;
            } else {
                groundResistance = -glm::normalize(glm::vec3(velocity.x, 0, velocity.z)) * groundFriction * normalForce;
            }
        } else {
            groundResistance = -glm::vec3(velocity.x, 0, velocity.z) * mass * frameRate;
        }
    } else {
        groundResistance = glm::vec3(0);
    }
    if (glm::length(velocity) == 0) {
        groundResistance = glm::vec3(0);
        airResistance = glm::vec3(0);
    }
    velocity += (airResistance + groundResistance + gravityForce) / (mass * frameRate);
}
