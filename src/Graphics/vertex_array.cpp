#include "vertex_array.h"
#include "graphics_api.h"
#include "gl_graphics_api.h"
#include "Core/logger.h"

namespace Lynx {

	std::unique_ptr<VertexArray> VertexArray::Create()
	{
		switch( IRendererAPI::GetAPI() ) {
		case API_OPENGL: return std::make_unique<GLVertexArray>();
		default : return nullptr;
		}
	}

}
