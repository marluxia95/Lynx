#ifndef GLELEMENTBUFFER_H
#define GLELEMENTBUFFER_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "Graphics/elementBuffer.h"

namespace Lynx::Graphics::OpenGL {

    class GLElementBuffer : Lynx::Graphics::ElementBuffer {

        public:
            GLElementBuffer(const void* indices, unsigned int indexNumber);
            GLElementBuffer(std::vector<GLuint>* indices);
            ~GLElementBuffer();
            virtual void Bind();
            virtual void Unbind();
            unsigned int ID;

    };

}

#endif