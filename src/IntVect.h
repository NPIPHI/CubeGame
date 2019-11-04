//
// Created by 16182 on 10/31/2019.
//

#ifndef MINECROFT_INTVECT_H
#define MINECROFT_INTVECT_H


#include <glm/vec3.hpp>

struct intVect{
    int x;
    int y;
    int z;
    intVect() = default;
    intVect(int x, int y, int z);
    intVect(glm::vec3 vect);
    intVect(int value);

    glm::vec3 vec3() const;
    intVect operator+(const intVect &addVect) const;
    intVect operator-(const intVect &minusVect) const;
    intVect operator/(const double scalar) const;
    intVect operator*(const double scalar) const;
    intVect operator%(const int modulo) const;
};

glm::vec3 intCast(const glm::vec3 vector);


#endif //MINECROFT_INTVECT_H
