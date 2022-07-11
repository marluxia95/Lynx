#include <glm/gtx/string_cast.hpp>
#include "Platform/OpenGL/gl_graphics_api.h"
#include "renderer_forward.h"
#include "Core/application.h"
#include "graphics_api.h"
#include "skybox.h"

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

        // Load needed shaders 
        m_objectShader = core_singleton->GetResourceManager()->LoadShader("res/shaders/texture.vert", "res/shaders/texture.frag");
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
        m_renderQueue.push(render_queue_obj{ renderable->GetMaterial(), renderable->GetMesh(), modelMatrix });
    }

    void ForwardRenderer::renderSky()
    {
        if(!m_skybox)
            return;

        auto cube_shader  = m_skybox->GetShader();
        auto cube_texture = m_skybox->GetTexture();
        
        glDepthFunc(GL_LEQUAL);

        cube_shader->Use();
        cube_shader->SetUniform("projection", m_camera->GetProjection());
        cube_shader->SetUniform("view", glm::mat4(glm::mat3(m_camera->UpdateView())) );
        
        cube_texture->Use();

        m_skybox->GetMesh()->VAO->Bind();
        m_skybox->GetMesh()->EBO->Bind();

        glCullFace(GL_BACK);
		RendererAPI::DrawIndexed(m_skybox->GetMesh()->indices->size());
        glCullFace(GL_FRONT);
        glDepthFunc(GL_LESS);
    }

    void ForwardRenderer::renderObjects()
    {
        Application* applicationInstance = Application::GetSingleton();
        m_objectShader->Use();
        m_objectShader->SetUniform("projection", m_camera->GetProjection());
        m_objectShader->SetUniform("view", m_camera->UpdateView() );
        m_objectShader->SetUniform("view_pos", m_camera->position);

        while (!m_renderQueue.empty()) {
            auto obj = m_renderQueue.top();
            m_objectShader->SetUniform("model", obj.transform);

            if(obj.mat.texture->IsValid())
                obj.mat.texture->Use();
            
            obj.mesh->VAO->Bind();
            obj.mesh->EBO->Bind();
		    RendererAPI::DrawIndexed(obj.mesh->indices->size());
            m_renderQueue.pop();
        }
    }   

    void ForwardRenderer::Render()
    {
        RendererAPI::Clear(glm::vec4(0));
        
        renderObjects();

        renderSky();
    }

    void ForwardRenderer::SetSkybox(std::shared_ptr<Skybox> skybox)
    {
        m_skybox = skybox;
    }


}