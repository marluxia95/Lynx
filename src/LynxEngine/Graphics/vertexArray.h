#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include "lynx_common.h"

namespace Lynx::Graphics {

	class LYNXENGINE_API VertexArray {
	public:
		VertexArray();
		~VertexArray();
		void Bind();
		void Unbind();
		GLuint VAO_ID;
	};

}

#endif