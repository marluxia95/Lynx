#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <stdio.h>
#include <memory>
#include <stdint.h>
#include <glm/glm.hpp>

namespace Lynx::Graphics {

    enum API {
        API_OPENGL
    };

    // Base class of the renderer API's, must never be instanced
    class RendererAPI {
        public:
            virtual ~RendererAPI();

            virtual void Init() = 0;

            virtual void SetViewport(uint32_t width, uint32_t height) = 0;
            virtual void Clear(glm::vec4 color) = 0;

            virtual void RenderIndexed() = 0;

            static API GetAPI() { return m_API; };
            static std::unique_ptr<RendererAPI> Create();
        private:
            static API m_API;
    };

    // Static instance of the renderer API
    class IRendererAPI {
        public:
            static void Init();

            static void SetViewport(uint32_t width, uint32_t height);
            static void Clear(glm::vec4 color);

            static void RenderIndexed();

        private:
            static RendererAPI m_rendererAPI;
    }
}

#endif