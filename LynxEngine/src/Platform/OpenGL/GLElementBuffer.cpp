#include <GL/glew.h> 
#include <iostream>
#include <vector>
#include "GLElementBuffer.h"

namespace Lynx::Graphics::OpenGL {

    GLElementBuffer::ElementBuffer(const void* indices, unsigned int indexNumber){
        glGenBuffers(1, &ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber, indices, GL_STATIC_DRAW);
    }

    GLElementBuffer::ElementBuffer(std::vector<GLuint>* indices){
        glGenBuffers(1, &ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), &(indices->at(0)), GL_STATIC_DRAW );
    }

    GLElementBuffer::~ElementBuffer(){
        glDeleteBuffers(1, &ID);
    }

    void GLElementBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void GLElementBuffer::Unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }

}