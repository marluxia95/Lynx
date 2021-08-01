#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"

namespace Lynx {

    class PerspectiveCamera : public Camera {
        public:
            PerspectiveCamera(glm::mat4 projection) : projectionMatrix(projection) { }

            void Update();

            void SetFOV( float fov ) { m_fov = fov; }

        private:
           float m_fov = 60.0f; 

    }

}

#endif