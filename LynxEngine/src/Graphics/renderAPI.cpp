#include <memory>
#include <cassert>
#include "Platform/OpenGL/GLRendererAPI.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {

API RendererAPI::m_API = API_OPENGL;

std::unique_ptr<RendererAPI> RendererAPI::Create(API api)
{
    switch(api) {
        case API_OPENGL : return std::make_unique<GLRendererAPI>();
    }
}

void IRendererAPI::Init()
{
    if(!m_rendererAPI)
        m_rendererAPI = RendererAPI::Create();
    
    m_rendererAPI->Init();
}

void IRendererAPI::Clear(glm::vec3 color)
{
    m_rendererAPI->Clear(color);
}

void IRendererAPI::RenderIndexed()
{
    m_rendererAPI->RenderIndexed();
}

}