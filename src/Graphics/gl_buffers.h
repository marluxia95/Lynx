#ifndef GLBUFFERS_H
#define GLBUFFERS_H

#include <vector>
#include <glm/glm.hpp>
#include "Graphics/buffers.h"
#include "Graphics/vertex_array.h"
#include "Graphics/mesh.h" // For mesh types

namespace Lynx::Graphics::OpenGL {

    class GLVertexBuffer : public Lynx::Graphics::VertexBuffer {
        public:
            GLVertexBuffer(const void* data, unsigned int size);
            GLVertexBuffer(std::vector<Vertex>* vertices);
            GLVertexBuffer(std::vector<Vertex>* vertices, MeshType type);
            GLVertexBuffer(unsigned int size);
            GLVertexBuffer();
            ~GLVertexBuffer();
            void SetData(const void* data, unsigned int size);
            void AddData(const void* data, unsigned int size);
            void DynamicDraw(bool dynamic_draw);
			void Bind();
            void Unbind();
            void Configure(MeshType type);
            unsigned int VBO_ID;
		private:
			bool dynamic; // GL_DYNAMIC_DRAW
    };

    class GLElementBuffer : public Lynx::Graphics::ElementBuffer {
        public:
            GLElementBuffer(const void* indices, unsigned int indexNumber);
            GLElementBuffer(std::vector<unsigned int>* indices);
            GLElementBuffer(unsigned int size);
            GLElementBuffer();
            ~GLElementBuffer();
			void SetData(const void* data, unsigned int size);
            void AddData(const void* data, unsigned int size);
			void DynamicDraw(bool dynamic_draw);
			void Bind();
            void Unbind();
            unsigned int ID;
		private:
			bool dynamic;
    };

}


#endif
