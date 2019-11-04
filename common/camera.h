//
// Created by 16182 on 7/16/2019.
//

#ifndef TUTORIALS_CAMERA_H
#define TUTORIALS_CAMERA_H

#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec3.hpp>


class camera {
private:
    glm::mat4 view;
    glm::mat4 perspective;
    glm::vec3 up;
    void generateView();

protected:
    glm::mat4 rotation;
public:
    glm::vec3 position;

    camera(float FOV, float aspectRatio, float nearPlane, float farPlane);
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 axis, float degrees);
    void rotate(glm::vec3 axis, float degrees);
    void lookAt(glm::vec3 focus);
    void translate(glm::vec3 delta);

    const glm::mat4 getView() const;
};


#endif //TUTORIALS_CAMERA_H
