//
// Created by 16182 on 10/31/2019.
//

#include "IntVect.h"

glm::vec3 intVect::operator+(const intVect &addVect) const{
    return glm::vec3(addVect.x + x, addVect.y + y, addVect.z + z);
}

intVect intVect::operator-(const intVect &minusVect) const{
    return intVect(x - minusVect.x, y - minusVect.y, z - minusVect.z);
}

intVect::intVect(int x, int y, int z){
    this->x = x;
    this->y = y;
    this->z = z;
}

intVect intVect::operator/(const double scalar) const{
    return intVect(x / scalar, y / scalar, z / scalar);
}

intVect intVect::operator*(const double scalar) const{
    return intVect(x * scalar, y * scalar, z * scalar);
}

intVect intVect::operator%(const int modulo) const{
    return intVect(x%modulo, y%modulo, z%modulo);
}

intVect::intVect(glm::vec3 vect){
    x = vect.x;
    y = vect.y;
    z = vect.z;
}