//
// Created by 16182 on 11/4/2019.
//

#ifndef MINECROFT_GEOMETRY_H
#define MINECROFT_GEOMETRY_H

#include <GLM/vec3.hpp>
#include "IntVect.h"

struct ejectData {
    glm::vec3 normal;
    float distance;
    bool intersects;
    bool corner;
    static constexpr float cornerThreashold = 0.15;
};

class axisCube {
public:
    glm::vec3 minCorner, maxCorner;

    axisCube(const glm::vec3 c1, const glm::vec3 c2);

    ejectData intersect(const axisCube &other) const;

    void setPos(glm::vec3 position);

    void translate(glm::vec3 delta);

    void operator+=(const glm::vec3 delta);

private:

};

axisCube unitCube(intVect minCorner);

static const glm::vec3 axises[] = {
        {-1, 0,  0},
        {0,  -1, 0},
        {0,  0,  -1},
        {1,  0,  0},
        {0,  1,  0},
        {0,  0,  1}
};

#endif //MINECROFT_GEOMETRY_H
