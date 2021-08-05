#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/ECS/components.h"
#include "Core/ECS/entity.h"
#include "Systems/cameraSystem.h"

namespace Lynx {

    
    // Base camera component
    class Camera {
    public:
        Camera(float resX, float resY) : resX(resX), resY(resY) { }

        void CalculateView(Transform* transform);
        virtual void CalculateProjection(float near=0.0f, float far=100.0f) = 0;

        inline glm::mat4 GetProjectionMatrix() { return projectionMatrix; }
        inline glm::mat4 GetViewMatrix() { return viewMatrix; }

        inline void SetProjectionMatrix(glm::mat4 proj) { projectionMatrix = proj; }

        float resX, resY = 0;
        bool main = true;
        friend class cameraSystem;
    protected:
        glm::mat4 projectionMatrix, viewMatrix = glm::mat4(1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    };

    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(float resX, float resY) : Camera(resX, resY) { }

        void CalculateView(Transform* transform);
        void CalculateProjection(float near = -1.0f, float far=1.0f);

    };

    class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera(float resX, float resY, float FOV=60.0f) : Camera(resX, resY), FOV(FOV) { }

        void CalculateView(Transform* transform);
        void CalculateProjection(float near = 0.0f, float far=1000.0f);

        float FOV;

    };


}



#endif