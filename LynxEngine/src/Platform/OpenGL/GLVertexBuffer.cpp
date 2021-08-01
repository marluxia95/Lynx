#include <GL/glew.h> 	
#include <iostream>
#include <vector>
#include "Core/logger.h"
#include "GLVertexBuffer.h"

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

    void GLVertexBuffer::Configure(Lynx::Graphics::MeshType type)
    {
        switch( type ){

            case MESH_2D_SPRITE:
                // Vertex attribute
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
                glEnableVertexAttribArray(0);
                break;
            case MESH_3D:
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
                glEnableVertexAttribArray(0);
                break;
            case MESH_3D_NORMAL:
                // Vertex attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
                glEnableVertexAttribArray(0);
                // Normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
                glEnableVertexAttribArray(1);
                break;
            case MESH_3D_TEXTURED:
                // Vertex attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
                glEnableVertexAttribArray(0);
                // Texture attribute
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
                glEnableVertexAttribArray(2);
                break;
            case MESH_3D_TEXTURED_NORMAL:
                // Vertex attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
                glEnableVertexAttribArray(0);
                // Normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
                glEnableVertexAttribArray(1);
                // Texture attribute
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
                glEnableVertexAttribArray(2);
                break;
        }
    }

}