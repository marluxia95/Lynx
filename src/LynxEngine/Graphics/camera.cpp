#include "camera.h"

namespace Lynx{

    Camera::Camera()
    {
        UpdateView();
    }

    Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : position(position), rotation(rotation), up(up)
    {
        UpdateView();
    }

    void Camera::CalcPerspective(float resX, float resY, float near, float far)
    {
        projection = glm::perspective(glm::radians(FOV), resX / resY, near, far);
    }

    void Camera::CalcOrthographic(float left, float right, float bottom, float top, float near, float far)
    {
        projection = glm::ortho(left, right, bottom, top, near, far);
    }

    glm::mat4 Camera::GetProjection()
    {
        return projection;
    }

    glm::mat4 Camera::UpdateView()
    {
        view = glm::lookAt(position, position + rotation, up);
        return view;
    }

    glm::vec3 Camera::GetPosition()
    {
        return position;
    }

    void Camera::SetPosition(glm::vec3 pos)
    {
        position = pos;
    }

    glm::vec3 Camera::GetRotation()
    {
        return rotation;
    }

    void Camera::SetRotation(glm::vec3 rot)
    {
        rotation = rot;
    }
}