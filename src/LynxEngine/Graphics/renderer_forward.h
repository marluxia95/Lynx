#ifndef RENDERER_FORWARD_H
#define RENDERER_FORWARD_H

#include "renderer.h"

namespace Lynx::Graphics {

    class Shader;
    class Skybox;
    
    class LYNXENGINE_API ForwardRenderer : public Renderer {
    private:
        void renderSky();
        void renderObjects();
        
        void processLighting(render_queue_obj renderObj);
    public:
        ForwardRenderer();
        
        ~ForwardRenderer();
        
        void Initialise();

        void SetCamera(Camera* camera) { m_camera = camera; }

        Camera* GetCamera() { return m_camera; }

        void Update();

        void Shutdown();

        void PushRender(Entity* ent);
        void PushRender(Renderable* renderable, glm::mat4 modelMatrix);

        void PushLight(PointLight* light);
        void SetDirectionalLight(DirectionalLight* light);

        void SetSkybox(std::shared_ptr<Skybox> skybox);

        void Render();
    private:
        std::shared_ptr<Shader> m_objectShader;
        std::shared_ptr<Skybox> m_skybox;

    };

}

#endif
