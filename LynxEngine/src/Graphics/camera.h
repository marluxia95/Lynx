#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <glm/glm.hpp>
#include "Core/entity.hpp"

namespace Lynx {

    // Base camera component
    class Camera {

        public:
            Camera(glm::mat4 projection, Entity* entity ) : projectionMatrix(projection), cameraEntity(entity) { }

            virtual void Update() { };

            inline glm::mat4 GetProjectionMatrix() { return projectionMatrix; }
            inline glm::mat4 GetViewMatrix() { return viewMatrix }

            inline void SetProjectionMatrix(glm::mat4 proj) { projectionMatrix = proj; }

        protected:
            glm::mat4 projectionMatrix, viewMatrix = glm::mat4(1.0f);
            Entity* cameraEntity; // Reference to the base camera entity so it can access data like the camera's transform

            


    };


}



#endif