#include <glm/gtx/string_cast.hpp>
#include "Platform/OpenGL/gl_graphics_api.h"
#include "renderer_forward.h"
#include "Core/application.h"
#include "Core/entity.h"
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
        m_objectShader = core_singleton->GetResourceManager()->LoadShader("res/shaders/lighting.vert", "res/shaders/lighting.frag");
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

    void ForwardRenderer::PushLight(PointLight& light)
    {
        m_pointLights.push_back(light);
    }

    void ForwardRenderer::SetDirectionalLight(DirectionalLight& light)
    {
        m_directionalLight = light;
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

    // Setup the point light array uniform values
    void ForwardRenderer::processLighting()
    {
        m_objectShader->SetUniformf("numPointLights", (int)m_pointLights.size());
        for(int l = 0; l < m_pointLights.size(); l++) {
            auto pointLight = m_pointLights[l];
            m_objectShader->SetUniformf("pointLights[%d].position", pointLight.Position, l);
            m_objectShader->SetUniformf("pointLights[%d].constant", pointLight.Constant, l);
            m_objectShader->SetUniformf("pointLights[%d].linear",   pointLight.Linear,   l);
            m_objectShader->SetUniformf("pointLights[%d].quadratic",pointLight.Quadratic,l);
            m_objectShader->SetUniformf("pointLights[%d].ambient",  pointLight.Ambient,  l);
            m_objectShader->SetUniformf("pointLights[%d].diffuse",  pointLight.Diffuse,  l);
            m_objectShader->SetUniformf("pointLights[%d].specular", pointLight.Specular, l);
        }

        m_objectShader->SetUniform("directionalLight.direction",    m_directionalLight.Direction);
        m_objectShader->SetUniform("directionalLight.ambient",      m_directionalLight.Ambient);
        m_objectShader->SetUniform("directionalLight.diffuse",      m_directionalLight.Diffuse);
        m_objectShader->SetUniform("directionalLight.specular",     m_directionalLight.Specular);
        m_objectShader->SetUniform("direcitonalLight.intensity",    m_directionalLight.Intensity);
        API_CheckErrors();
    }

    // Object rendering process
    void ForwardRenderer::renderObjects()
    {
        Application* applicationInstance = Application::GetSingleton();
        m_objectShader->Use();
        m_objectShader->SetUniform("projection", m_camera->GetProjection());
        m_objectShader->SetUniform("view", m_camera->UpdateView() );
        m_objectShader->SetUniform("viewPos", m_camera->position);

        while (!m_renderQueue.empty()) {
            render_queue_obj obj = m_renderQueue.top();
            Material mat = obj.mat;

            m_objectShader->SetUniform("model", obj.transform);
            m_objectShader->SetUniform("material.ambient",      mat.ambient);
            m_objectShader->SetUniform("material.diffuse",      mat.diffuse);
            m_objectShader->SetUniform("material.specular",     mat.specular);
            m_objectShader->SetUniform("material.shininess",    mat.shininess);

            if(mat.texture_diffuse){
                m_objectShader->SetUniform("diffuse_map", true);
                m_objectShader->SetUniform("material.diffuse_tex",  0);
                mat.texture_diffuse->Use();
            }

            if(mat.texture_specular){
                m_objectShader->SetUniform("specular_map", true);
                m_objectShader->SetUniform("material.specular_tex", 0);
                mat.texture_specular->Use();
            }

            processLighting();

            if(mat.texture)
                mat.texture->Use();

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
