#include <stack>
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

    void ForwardRenderer::PushRender(Entity* ent)
    {
        if (ent == NULL)
            return;

        std::stack<Entity*> ent_stack;
        
        ent_stack.push(ent);
        
        for (int i = 0; i < ent->GetChildrenCount(); i++) 
            ent_stack.push(ent->GetChildByIndex(i));
        
        while (!ent_stack.empty()) {
            Entity* s_ent = ent_stack.top();
            
            if (!s_ent->IsRenderable()) {
                ent_stack.pop();
                continue;
            }
            
            PushRender(s_ent->GetRenderHndl(), s_ent->GetModelMatrix());
            ent_stack.pop();
        }

    }
    
    void ForwardRenderer::PushRender(Renderable* renderable, glm::mat4 modelMatrix)
    {
        m_renderQueue.push_back(render_queue_obj{ renderable->GetMaterial(), renderable->GetMesh(), modelMatrix });
    }

    void ForwardRenderer::renderSky()
    {

    }

    void ForwardRenderer::renderObjects()
    {
        for(auto& obj : m_renderQueue) {
            
        }
        m_renderQueue.clear();
    }   

    void ForwardRenderer::Render()
    {
        renderSky();
        renderObjects();
    }


}