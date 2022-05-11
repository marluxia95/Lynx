#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include "lynx_common.h"

namespace Lynx::Graphics {

	class LYNXENGINE_API VertexArray {
	public:
		VertexArray() = default;
		~VertexArray() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		unsigned int VAO_ID;

		static std::unique_ptr<VertexArray> Create();
	};

}

#endif