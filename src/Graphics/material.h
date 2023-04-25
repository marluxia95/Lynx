#ifndef MATERIAL_H
#define MATERIAL_H

#include "Core/resource.h"
#include "texture.h"
#include "shader.h"

namespace Lynx::Graphics {

    class Material : public Resource {
    public:
        Material() : ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(1.0f)) {}
        Material(std::shared_ptr<Graphics::TextureBase> texture) : ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(1.0f)), texture(texture) {}
        Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, std::shared_ptr<Graphics::Shader> shader) : ambient(a), diffuse(d), specular(s), shader(shader) {}
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
        std::shared_ptr<Graphics::Shader> shader;
	    std::shared_ptr<Graphics::TextureBase> texture;
	    std::shared_ptr<Graphics::TextureBase> texture_diffuse;
	    std::shared_ptr<Graphics::TextureBase> texture_specular;
    };
}

#endif
