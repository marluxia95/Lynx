#include <stdio.h>
#include <GL/glew.h> 
#include "GLVertexArray.h"

namespace Lynx::Graphics::OpenGL {

    VertexArray::VertexArray(){
        glGenVertexArrays(1, &VAO_ID);
        glBindVertexArray(VAO_ID);
    }

    VertexArray::~VertexArray(){
        glDeleteVertexArrays(1, &VAO_ID);
    }

    void VertexArray::Bind(){
        glBindVertexArray(VAO_ID);
    }

    void VertexArray::Unbind(){
        glBindVertexArray(0);
    }

    void SetElementBuffer(std::shared_ptr<ElementBuffer> elementBuffer)
    {
        
    }

}