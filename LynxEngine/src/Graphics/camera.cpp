#include "camera.h"

namespace Lynx {

    void Camera::CalculateView(Transform* transform)
    {
        viewMatrix = lookAt(transform->position, transform->position + transform->rotation, up);
    }

    void OrthographicCamera::CalculateProjection(float near, float far)
    {
        projectionMatrix = ortho(0.0f, (float)resX / (float)resY, 0.0f, 1.0f, near, far);
    }

    void PerspectiveCamera::CalculateProjection(float near, float far)
    {
        projectionMatrix = perspective(radians(FOV), (float)resX / (float)resY, near, far); 
    }



    

}