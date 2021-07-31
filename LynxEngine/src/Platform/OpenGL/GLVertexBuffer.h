#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Graphics/vertexBuffer.h"

using namespace glm;

namespace Lynx::Graphics::OpenGL {

    class GLVertexBuffer : Lynx::Graphics::VertexBuffer {
        public:
            GLVertexBuffer(const void* data, unsigned int size);
            GLVertexBuffer(std::vector<Vertex>* vertices);
            ~GLVertexBuffer();
            void Bind();
            void Unbind();
            unsigned int VBO_ID;
    };

}

#endif