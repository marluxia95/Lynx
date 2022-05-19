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

	std::unique_ptr<VertexBuffer> VertexBuffer::Create()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLVertexBuffer>();
			default : return nullptr;
		}
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Create(const void* data, unsigned int size)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLVertexBuffer>(data, size);
			default : return nullptr;
		}
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Create(unsigned int size)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLVertexBuffer>(size);
			default : return nullptr;
		}
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>* vertices)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLVertexBuffer>(vertices);
			default : return nullptr;
		}
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>* vertices, MeshType type)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLVertexBuffer>(vertices, type);
			default : return nullptr;
		}
	}

	std::unique_ptr<ElementBuffer> ElementBuffer::Create()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLElementBuffer>();
			default : return nullptr;
		}
	}

	std::unique_ptr<ElementBuffer> ElementBuffer::Create(unsigned int size)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLElementBuffer>(size);
			default : return nullptr;
		}
	}

	std::unique_ptr<ElementBuffer> ElementBuffer::Create(const void* indices, unsigned int indexNumber)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLElementBuffer>(indices, indexNumber);
			default : return nullptr;
		}
	}

    std::unique_ptr<ElementBuffer> ElementBuffer::Create(std::vector<GLuint>* indices)
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLElementBuffer>(indices);
			default : return nullptr;
		}
	}

}