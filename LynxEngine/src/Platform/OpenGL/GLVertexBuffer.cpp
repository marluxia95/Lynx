#include <GL/glew.h> 	
#include <iostream>
#include <vector>
#include "GLVertexBuffer.h"
#include "Core/logger.h"

namespace Lynx::Graphics::OpenGL {

    GLVertexBuffer::VertexBuffer(const void* data, unsigned int size){
        glGenBuffers(1, &VBO_ID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    GLVertexBuffer::VertexBuffer(std::vector<Vertex>* vertices){
        glGenBuffers(1, &VBO_ID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
        glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->at(0), GL_STATIC_DRAW);
    }

    GLVertexBuffer::~VertexBuffer(){
        glDeleteBuffers(1, &VBO_ID);
    }

    void GLVertexBuffer::Bind(){
        glBindBuffer(GL_ARRAY_BUFFER, VBO_ID); 
    }

    void GLVertexBuffer::Unbind(){
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
    }

}