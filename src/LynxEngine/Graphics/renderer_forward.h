#ifndef RENDERER_FORWARD_H
#define RENDERER_FORWARD_H

#include "renderer.h"

namespace Lynx::Graphics {
    
    class ForwardRenderer : public Renderer {
    public:
        ForwardRenderer();
        
        ~ForwardRenderer();
        
        void Initialise();

        void SetCamera(Camera* camera) { m_camera = camera; }

        Camera* GetCamera() { return m_camera; }

        void Update();

        void Shutdown();

        void PushRender(Entity ent);
        void PushRender(Renderable* renderable, glm::mat4 modelMatrix);

        void Render();
    private:
        void renderSky();
        void renderObjects();

    };

}

#endif