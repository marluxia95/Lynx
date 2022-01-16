#include <GL/glew.h> 	
#include <iostream>
#include <vector>
#include "buffers.h"
#include "Core/logger.h"


namespace Lynx::Graphics {

	void debugVBO(std::vector<Vertex>* vertices, std::vector<GLuint>* indices)
	{
		for ( int i = 0; i < indices->size(); i++ ){
			log_debug("Index Number %d Vertex pos: %f %f %f Texture Coord pos %f %f Normal pos %f %f %f\n", i, vertices->at(indices->at(i)).Position.x, 
			vertices->at(indices->at(i)).Position.y, 
			vertices->at(indices->at(i)).Position.z,
			vertices->at(indices->at(i)).TextureCoords.x,
			vertices->at(indices->at(i)).TextureCoords.y,
			vertices->at(indices->at(i)).Normal.x,
			vertices->at(indices->at(i)).Normal.y,
			vertices->at(indices->at(i)).Normal.z);
		}
	}

	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

    VertexBuffer::VertexBuffer(std::vector<Vertex>* vertices)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(vertices->at(0)), GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}
    
	VertexBuffer::VertexBuffer(std::vector<Vertex>* vertices, MeshType type)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->at(0), GL_STATIC_DRAW);
		Configure(type);
		log_debug("[GL] Initialized vertex buffer");
	}
    
	VertexBuffer::VertexBuffer(unsigned int size)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

	VertexBuffer::VertexBuffer()
	{	
		glGenBuffers(1, &VBO_ID);
		log_debug("[GL] Initialized vertex buffer");
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &VBO_ID);
	}

	void VertexBuffer::SetData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::AddData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID); 
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
	}

	void VertexBuffer::Configure(MeshType type)
	{
		switch( type ){

			case MESH_2D_SPRITE:
				// Vertex attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
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
	    		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
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
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
				glEnableVertexAttribArray(2);
				break;
		}
	}
 
    ElementBuffer::ElementBuffer(const void* indices, unsigned int indexNumber){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber, indices, GL_STATIC_DRAW);
		log_debug("[GL] Initialized element buffer");
    }

    ElementBuffer::ElementBuffer(std::vector<unsigned int>* indices){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &(indices->at(0)), GL_STATIC_DRAW );
		log_debug("[GL] Initialized element buffer");
    }

    ElementBuffer::ElementBuffer(unsigned int size){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW );
		log_debug("[GL] Initialized element buffer");
    }

    ElementBuffer::ElementBuffer()
    {
        glGenBuffers(1, &ID);
		log_debug("[GL] Initialized element buffer");
    }

    ElementBuffer::~ElementBuffer(){
        glDeleteBuffers(1, &ID);
    }

    void ElementBuffer::AddData(const void* data, unsigned int size)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);  
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
    }

    void ElementBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void ElementBuffer::Unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }

}