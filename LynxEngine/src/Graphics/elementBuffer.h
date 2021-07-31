#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include <iostream>
#include <vector>

namespace Lynx::Graphics {

    class ElementBuffer {

        public:
            ElementBuffer(const void* indices, unsigned int indexNumber) = default;
            ElementBuffer(std::vector<GLuint>* indices) = default;
            ~ElementBuffer() = default;
            virtual void Bind() = 0;
            virtual void Unbind() = 0;
            unsigned int ID;

    };

}

#endif