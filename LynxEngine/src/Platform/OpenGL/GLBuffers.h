#ifndef GLBUFFERS_H
#define GLBUFFERS_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Graphics/buffers.h"
#include "Graphics/vertexArray.h"
#include "Graphics/mesh.h" // For mesh types

namespace Lynx::Graphics::OpenGL {

    class GLVertexBuffer : public Lynx::Graphics::VertexBuffer {
        public:
            GLVertexBuffer(const void* data, unsigned int size);
            GLVertexBuffer(vector<Vertex>* vertices);
            GLVertexBuffer(vector<Vertex>* vertices, MeshType type);
            GLVertexBuffer(unsigned int size);
            GLVertexBuffer();
            ~GLVertexBuffer();
            void SetData(const void* data, unsigned int size);
            void AddData(const void* data, unsigned int size);
            void Bind();
            void Unbind();
            void Configure(MeshType type);
            unsigned int VBO_ID;
    };

    class GLElementBuffer : public Lynx::Graphics::ElementBuffer {
        public:
            GLElementBuffer(const void* indices, unsigned int indexNumber);
            GLElementBuffer(vector<unsigned int>* indices);
            GLElementBuffer(unsigned int size);
            GLElementBuffer();
            ~GLElementBuffer();
            void AddData(const void* data, unsigned int size);
            void Bind();
            void Unbind();
            unsigned int ID;
    };

}


#endif
