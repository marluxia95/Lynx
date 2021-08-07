#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/logger.h"
#include "mesh.h"

using namespace glm;

namespace Lynx::Graphics {

    enum MeshType : unsigned int;

    struct Vertex {
        vec3 Position;
        vec2 TextureCoords;
        vec3 Normal;
    };

    void debugVBO(std::vector<Vertex>* vertices, std::vector<GLuint>* indices);

    class VertexBuffer {
        public:
            virtual void Configure(MeshType type) = 0;
            virtual void Bind() = 0;
            virtual void Unbind() = 0;
            unsigned int VBO_ID;
    };

}

#endif