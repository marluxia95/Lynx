#include <stdio.h>
#include <GL/glew.h> 
#include "GLVertexArray.h"


namespace Lynx::Graphics::OpenGL {

    GLVertexArray::GLVertexArray(){
        glGenVertexArrays(1, &VAO_ID);
        glBindVertexArray(VAO_ID);
    }

    GLVertexArray::~GLVertexArray(){
        glDeleteVertexArrays(1, &VAO_ID);
    }

    void GLVertexArray::Bind(){
        glBindVertexArray(VAO_ID);
    }

    void GLVertexArray::Unbind(){
        glBindVertexArray(0);
    }

}