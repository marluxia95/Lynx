#ifndef CAMERA_H
#define CAMERA_H

#include "Core/entity.h"

namespace Lynx {

    class LYNXENGINE_API Camera {
    private:
        glm::vec3 position = glm::vec3(0,0,0);
        glm::vec3 forward = glm::vec3(0,0,-1);
        glm::vec3 up = glm::vec3(0, 1, 0);

        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);

        float FOV = 60;
    public:
        Camera();
        Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up);
        void CalcPerspective(float resX, float resY, float near, float far);
        void CalcOrthographic(float left, float right, float bottom, float top, float near, float far);
        glm::mat4 GetProjection();
        glm::mat4 UpdateView();
        glm::vec3 GetPosition();
        void SetPosition(glm::vec3 pos);
    };

}

#endif