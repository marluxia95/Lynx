#include <stdio.h>
#include <GL/glew.h> 
#include "Core/logger.h"
#include "gl_vertex_array.h"

namespace Lynx::Graphics::OpenGL {

	GLVertexArray::GLVertexArray(){
		glGenVertexArrays(1, &VAO_ID);
		glBindVertexArray(VAO_ID);
		log_debug("[GL] Initialized vertex array");
	}

	GLVertexArray::~GLVertexArray(){
		glDeleteVertexArrays(1, &VAO_ID);
		log_debug("[GL] Destroyed vertex array");
	}

	void GLVertexArray::Bind(){
		glBindVertexArray(VAO_ID);
	}

	void GLVertexArray::Unbind(){
		glBindVertexArray(0);
	}

}