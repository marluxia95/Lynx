#include <glm/gtx/string_cast.hpp>
#include "renderer_forward.h"
#include "Core/application.h"
#include "graphics_api.h"

namespace Lynx::Graphics {

    ForwardRenderer::ForwardRenderer() : Renderer()
    {

    }

    ForwardRenderer::~ForwardRenderer()
    {
        free(m_camera);
    }

    void ForwardRenderer::Initialise()
    {
        const auto core_singleton = Application::GetSingleton();
        RendererAPI::Init();
        RendererAPI::SetViewport(core_singleton->GetResolutionWidth(), core_singleton->GetResolutionHeight());

        // Load needed shaders 
        m_objectShader = core_singleton->GetResourceManager()->LoadShader("res/shaders/default.vert", "res/shaders/default.frag");
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
            log_debug("Pushed mesh with %d indices", s_ent->GetRenderHndl()->GetMesh()->indices->size());
            ent_stack.pop();
        }

    }
    
    void ForwardRenderer::PushRender(Renderable* renderable, glm::mat4 modelMatrix)
    {
        m_renderQueue.push(render_queue_obj{ renderable->GetMaterial(), renderable->GetMesh(), modelMatrix });
    }

    void ForwardRenderer::renderSky()
    {

    }

    void ForwardRenderer::renderObjects()
    {
        Application* applicationInstance = Application::GetSingleton();
        m_objectShader->Use();
        m_objectShader->SetUniform("projection", m_camera->GetProjection());
        m_objectShader->SetUniform("view", m_camera->UpdateView());
        m_objectShader->SetUniform("view_pos", m_camera->GetPosition());

        log_debug("Proj : %s", glm::to_string(m_camera->GetProjection()).c_str());
        log_debug("View : %s", glm::to_string(m_camera->UpdateView()).c_str());

        while (!m_renderQueue.empty()) {
            auto obj = m_renderQueue.top();
            m_objectShader->SetUniform("model", obj.transform);
            log_debug("Model : %s", glm::to_string(obj.transform).c_str());
            
            obj.mesh->VAO->Bind();
            obj.mesh->Render();
            m_renderQueue.pop();
        }
    }   

    void ForwardRenderer::Render()
    {
        renderSky();
        renderObjects();

        RendererAPI::Clear(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    }


}