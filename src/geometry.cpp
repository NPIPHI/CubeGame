//
// Created by 16182 on 11/4/2019.
//

#include <glm/glm.hpp>
#include "geometry.h"
#include <iostream>

ejectData axisCube::intersect(const axisCube &other) const {
    ejectData eject = {glm::vec3(0), 100000000.f, true, false};
    for (const glm::vec3 &axis: axises) {
        float thisMin, otherMax, axisDistance;
        thisMin = std::fmin(glm::dot(minCorner, axis), glm::dot(maxCorner, axis));
        otherMax = std::fmax(glm::dot(other.minCorner, axis), glm::dot(other.maxCorner, axis));
        axisDistance = otherMax - thisMin;
        if (fabsf(eject.distance - axisDistance) < ejectData::cornerThreashold) {
            eject.corner = true;
        };
        if (axisDistance < eject.distance) {
            if (fabsf(eject.distance - axisDistance) >= ejectData::cornerThreashold) {
                eject.corner = false;
            }

            if (axisDistance < 0) {
                eject.intersects = false;
            }

            eject.distance = axisDistance;
            eject.normal = axis;
        }
    }
    return eject;
}

void axisCube::setPos(glm::vec3 position) {
    maxCorner = maxCorner - minCorner + position;
    minCorner = position;
}

axisCube unitCube(intVect minCorner) {
    return axisCube(minCorner.vec3(), (minCorner + 1).vec3());
}

axisCube::axisCube(const glm::vec3 c1, const glm::vec3 c2) {
    minCorner = c1;
    maxCorner = c2;
}

void axisCube::translate(glm::vec3 delta) {
    minCorner += delta;
    maxCorner += delta;
}

void axisCube::operator+=(glm::vec3 delta) {
    minCorner += delta;
    maxCorner += delta;
}
