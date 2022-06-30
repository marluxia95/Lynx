#include "renderer_forward.h"
#include "Core/application.h"
#include "graphics_api.h"


namespace Lynx::Graphics {

    ForwardRenderer::ForwardRenderer() : Renderer()
    {

    }

    ForwardRenderer::~ForwardRenderer()
    {

    }

    void ForwardRenderer::Initialise()
    {
        const auto core_singleton = Application::GetSingleton();
        RendererAPI::Init();
        RendererAPI::SetViewport(core_singleton->GetResolutionWidth(), core_singleton->GetResolutionHeight());
    }

    void ForwardRenderer::Update()
    {
        
    }

    void ForwardRenderer::Shutdown()
    {

    }

    void ForwardRenderer::PushRender(Entity ent)
    {

    }
    
    void ForwardRenderer::PushRender(RenderHndlPtr renderable, glm::mat4 modelMatrix)
    {

    }

    void ForwardRenderer::renderSky()
    {

    }

    void ForwardRenderer::renderObjects()
    {
        for(auto& obj : m_renderQueue) {
            
        }
    }   

    void ForwardRenderer::Render()
    {

    }


}