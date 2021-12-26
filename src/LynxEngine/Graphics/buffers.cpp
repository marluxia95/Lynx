#include <GL/glew.h> 	
#include <iostream>
#include <vector>
#include "buffers.h"
#include "Core/logger.h"
#include "rendererAPI.h"
#include "Platform/OpenGL/GLBuffers.h"

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

	VertexBuffer* VertexBuffer::Create()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLVertexBuffer();
			default : return nullptr;
		}
	}

	VertexBuffer* VertexBuffer::Create(const void* data, unsigned int size)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLVertexBuffer(data, size);
			default : return nullptr;
		}
	}

	VertexBuffer* VertexBuffer::Create(unsigned int size)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLVertexBuffer(size);
			default : return nullptr;
		}
	}

	VertexBuffer* VertexBuffer::Create(std::vector<Vertex>* vertices)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLVertexBuffer(vertices);
			default : return nullptr;
		}
	}

	VertexBuffer* VertexBuffer::Create(std::vector<Vertex>* vertices, MeshType type)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLVertexBuffer(vertices, type);
			default : return nullptr;
		}
	}

	ElementBuffer* ElementBuffer::Create()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLElementBuffer();
			default : return nullptr;
		}
	}

	ElementBuffer* ElementBuffer::Create(unsigned int size)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLElementBuffer(size);
			default : return nullptr;
		}
	}

	ElementBuffer* ElementBuffer::Create(const void* indices, unsigned int indexNumber)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLElementBuffer(indices, indexNumber);
			default : return nullptr;
		}
	}

    ElementBuffer* ElementBuffer::Create(std::vector<GLuint>* indices)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new OpenGL::GLElementBuffer(indices);
			default : return nullptr;
		}
	}
}