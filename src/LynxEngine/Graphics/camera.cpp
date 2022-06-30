#include "camera.h"

namespace Lynx{

    Camera::Camera()
    {
        UpdateView();
    }

    Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : position(position), forward(forward), up(up)
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

    void Camera::UpdateView()
    {
        view = glm::lookAt(position, position + forward, up);
    }
}