#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <memory>

namespace Lynx::Graphics {

    class VertexArray {
        public:
            VertexArray() = default;
            ~VertexArray() = default;
            virtual void Bind() = 0;
            virtual void Unbind() = 0;
            unsigned int VAO_ID;
    };

}

#endif