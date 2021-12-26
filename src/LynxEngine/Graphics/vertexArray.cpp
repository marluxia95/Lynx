#include "vertexArray.h"
#include "rendererAPI.h"
#include "Platform/OpenGL/GLVertexArray.h"
#include "Core/logger.h"

namespace Lynx::Graphics {

    VertexArray* VertexArray::Create()
    {
        switch( IRendererAPI::GetAPI() ) {
            case API_OPENGL: return new OpenGL::GLVertexArray();
            default : return nullptr;
        }
    }
}