#ifndef GLVERTEXARRAY_H
#define GLVERTEXARRAY_H

#include "Graphics/vertex_array.h"

namespace Lynx::Graphics::OpenGL {

	class GLVertexArray : public Lynx::Graphics::VertexArray {
	public:
		GLVertexArray();
		~GLVertexArray();
		void Bind();
		void Unbind();
		unsigned int VAO_ID;
	};

}

#endif