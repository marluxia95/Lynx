#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Graphics/vertexBuffer.h"
#include "Graphics/mesh.h"

namespace Lynx::Graphics::OpenGL {

    class GLVertexBuffer : public Lynx::Graphics::VertexBuffer {
        public:
            GLVertexBuffer(const void* data, unsigned int size);
            GLVertexBuffer(std::vector<Vertex>* vertices);
            ~GLVertexBuffer();
            void Configure(Lynx::Graphics::MeshType type);
            void Bind();
            void Unbind();
            unsigned int VBO_ID;
    };

}

#endif