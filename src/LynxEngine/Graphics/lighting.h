#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>

namespace Lynx::Graphics {

    class PointLight {
    public:
        PointLight(glm::vec3 position, float constant, float linear, float quadratic) :
            Position(position), Constant(constant), Linear(linear), Quadratic(quadratic) {}
        PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic) :
            Position(position), Ambient(color), Diffuse(color), Specular(color), Constant(constant),
            Linear(linear), Quadratic(Quadratic) {}
        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Ambient, Diffuse, Specular = glm::vec3(1.0f);
        float Constant, Linear, Quadratic;
    };

    class DirectionalLight {
    public:
        glm::vec3 Direction = glm::vec3(0.0f);
        glm::vec3 Ambient, Diffuse, Specular = glm::vec3(1.0f);
        float Intensity = 2.0f;
    };

}

#endif // LIGHTING_H
