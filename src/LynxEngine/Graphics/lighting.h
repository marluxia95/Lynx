#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>

namespace Lynx::Graphics {

    class PointLight {
    public:
        glm::vec3 Position = glm::vec3(0.0f); 
        glm::vec3 Ambient, Diffuse, Specular = glm::vec3(1.0f);
        float Constant, Linear, Quadratic;
    };

    class DirectionalLight {
    public:
        glm::vec3 Direction = glm::vec3(0.0f);
        glm::vec3 Ambient, Diffuse, Specular = glm::vec3(1.0f);
    };

}

#endif // LIGHTING_H
