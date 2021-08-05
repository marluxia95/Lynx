#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include <iostream>
#include <vector>

namespace Lynx::Graphics {

    class ElementBuffer {

        public:
            ElementBuffer() = default;
            ~ElementBuffer() = default;
            virtual void Bind() = 0;
            virtual void Unbind() = 0;
            unsigned int ID;

    };

}

#endif