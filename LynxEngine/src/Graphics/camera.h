#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/ECS/components.h"
#include "Core/ECS/entity.h"
#include "Core/logger.h"
#include "Systems/cameraSystem.h"

namespace Lynx {

    enum CameraType {
        CAMERA_ORTHOGRAPHIC, 
        CAMERA_PERSPECTIVE
    };

    struct Camera {
        int width, height;
        CameraType type;
        float FOV;
        float near, far;
        bool main = true;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 view, projection;
    };

}



#endif