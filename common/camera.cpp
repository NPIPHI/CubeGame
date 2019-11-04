//
// Created by 16182 on 7/16/2019.
//

#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

camera::camera(float FOV, float aspectRatio, float nearPlane, float farPlane) {
    perspective = glm::perspective(FOV, aspectRatio, nearPlane, farPlane);
    setPosition(vec3(0, 0, 0));
    setRotation(vec3(0, 1, 0), 0);
}

void camera::setPosition(const vec3 position) {
    this->position = position;
    generateView();
}

const mat4 camera::getView() const {
    return view;
}

void camera::generateView() {
    view = perspective * glm::translate(rotation, -position);
}

void camera::setRotation(const vec3 axis, const float degrees) {
    rotation = glm::rotate(mat4(1.0), degrees, axis);
    generateView();
}

void camera::rotate(const vec3 axis, const float degrees) {
    rotation = glm::rotate(rotation, degrees, axis);
    generateView();
}

void camera::translate(const vec3 delta) {
    position += delta;
    generateView();
}

void camera::lookAt(const vec3 focus) {
    rotation = glm::lookAt(vec3(0, 0, 0), position - focus, vec3(0, 1, 0));
    generateView();
}
