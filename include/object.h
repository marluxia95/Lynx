#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <glm/glm.hpp>
#include "mesh.h"

namespace Lynx {

class Object3D : Mesh3D {
    public:
        Object3D(glm::vec3 LightPos, glm::vec3 LightColor);
        ~Object3D();
        void Render();
    protected:

};

}
#endif