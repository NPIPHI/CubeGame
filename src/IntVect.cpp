//
// Created by 16182 on 10/31/2019.
//

#include "IntVect.h"

glm::vec3 intCast(const glm::vec3 vector) {
    return glm::vec3(int(vector.x), int(vector.y), int(vector.z));
}

intVect intVect::operator+(const intVect &addVect) const {
    return intVect(addVect.x + x, addVect.y + y, addVect.z + z);
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

intVect::intVect(int value) {
    x = value;
    y = value;
    z = value;
}
