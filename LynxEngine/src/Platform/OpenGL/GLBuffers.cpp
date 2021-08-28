#include "GLBuffers.h"
#include "Core/logger.h"

namespace Lynx::Graphics::OpenGL {

    GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int size)
    {
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

    GLVertexBuffer::GLVertexBuffer(std::vector<Vertex>* vertices)
    {
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(vertices->at(0)), GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

    GLVertexBuffer::GLVertexBuffer(std::vector<Vertex>* vertices, MeshType type)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->at(0), GL_STATIC_DRAW);
		Configure(type);
		log_debug("[GL] Initialized vertex buffer");
	}

	GLVertexBuffer::GLVertexBuffer(unsigned int size)
	{
		glGenBuffers(1, &VBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		log_debug("[GL] Initialized vertex buffer");
	}

	GLVertexBuffer::GLVertexBuffer()
	{
		glGenBuffers(1, &VBO_ID);
		log_debug("[GL] Initialized vertex buffer");
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &VBO_ID);
	}

	void GLVertexBuffer::SetData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void GLVertexBuffer::AddData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void GLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID); 
	}

	void GLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
	}

	void GLVertexBuffer::Configure(MeshType type)
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
 
    GLElementBuffer::GLElementBuffer(const void* indices, unsigned int indexNumber){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber, indices, GL_STATIC_DRAW);
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::GLElementBuffer(std::vector<unsigned int>* indices){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &(indices->at(0)), GL_STATIC_DRAW );
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::GLElementBuffer(unsigned int size){
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW );
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::GLElementBuffer()
    {
        glGenBuffers(1, &ID);
		log_debug("[GL] Initialized element buffer");
    }

    GLElementBuffer::~GLElementBuffer(){
        glDeleteBuffers(1, &ID);
    }

    void GLElementBuffer::AddData(const void* data, unsigned int size)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);  
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
    }

    void GLElementBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void GLElementBuffer::Unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }

}
