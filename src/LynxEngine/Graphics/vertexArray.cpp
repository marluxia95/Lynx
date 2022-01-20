#include "vertexArray.h"
#include "Core/logger.h"

namespace Lynx::Graphics {

    VertexArray::VertexArray(){
		glGenVertexArrays(1, &VAO_ID);
		glBindVertexArray(VAO_ID);
		log_debug("[GL] Initialized vertex array");
	}

	VertexArray::~VertexArray(){
		glDeleteVertexArrays(1, &VAO_ID);
		log_debug("[GL] Destroyed vertex array");
	}

	void VertexArray::Bind(){
		glBindVertexArray(VAO_ID);
	}

	void VertexArray::Unbind(){
		glBindVertexArray(0);
	}
    
}