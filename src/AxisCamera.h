//
// Created by 16182 on 10/19/2019.
//

#ifndef MINECROFT_AXISCAMERA_H
#define MINECROFT_AXISCAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../common/camera.h"
#include "GameMap.h"

class AxisCamera : public camera {
public:
    explicit AxisCamera(float aspectRatio);

    AxisCamera();


    double xRot, yRot;
    void control(GLFWwindow * window);
    void setView(GLFWwindow *window, glm::vec3 target);

    void updateViewFromMouse(double mouseX, double mouseY);
    glm::vec3 rayCast(double distance); // cast a ray from the camera forward a distance
    glm::vec3 forewardVector();
    std::vector<glm::vec3> rayCastBlocks(double maxDistance); //return all axis bound cube the ray passes through, temp implementation
    glm::vec3 rayCastToBlock(double maxDistance, const GameMap& refMap);
    glm::vec3 rayCastToPreviousBlock(double maxDistance, const GameMap& refMap);

    glm::mat2 getXZtransform();
private:
    static constexpr double pi = 3.14159265358979;
    double mousePosX, mousePosY, mouseRefrenceX, mouseRefrenceY;
};


#endif //MINECROFT_AXISCAMERA_H
