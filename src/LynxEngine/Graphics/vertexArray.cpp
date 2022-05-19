#include "vertexArray.h"
#include "rendererAPI.h"
#include "Platform/OpenGL/GLVertexArray.h"
#include "Core/logger.h"

namespace Lynx::Graphics {

    std::unique_ptr<VertexArray> VertexArray::Create()
    {
        switch( IRendererAPI::GetAPI() ) {
            case API_OPENGL: return std::make_unique<OpenGL::GLVertexArray>();
            default : return nullptr;
        }
    }
    
}