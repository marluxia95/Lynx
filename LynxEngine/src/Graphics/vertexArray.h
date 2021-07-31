#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <memory>
#include "elementBuffer.h"

namespace Lynx::Graphics {

    class VertexArray {
        public:
            VertexArray() = default;
            ~VertexArray() = default;
            virtual void Bind() = 0;
            virtual void Unbind() = 0;
            virtual void SetElementBuffer(std::shared_ptr<ElementBuffer> elementBuffer) = 0;
            virtual std::shared_ptr<ElementBuffer> GetElementBuffer() = 0;
            unsigned int VAO_ID;
    };

}

#endif