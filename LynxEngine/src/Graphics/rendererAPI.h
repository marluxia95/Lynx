#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <stdio.h>
#include <stdbool.h>
#include <memory>
#include <stdint.h>
#include <glm/glm.hpp>

namespace Lynx::Graphics {

    enum BufferType : unsigned int {
        VERTEXBUF, ELEMENTBUF, FRAMEBUF
    };

    enum API : unsigned int {
        API_OPENGL
    };

    // Abstract base class of the renderer API's, should only be used as base for other platform API's
    class IRendererAPI {
        
        public:
            virtual void Init() = 0;

            virtual void SetViewport(uint32_t width, uint32_t height) = 0;
            virtual void Clear(glm::vec4 color) = 0;

            virtual void LoadTexture(unsigned char* data, int width, int height, bool useDefaults=true) = 0;
            virtual unsigned int GenTexture() = 0;
            virtual void SetTextureParameter(unsigned int param, unsigned int value) = 0;
            virtual void UseTexture(unsigned int id) = 0;
            virtual void BindTexture(unsigned int tex) = 0;

            virtual void CheckErrors() = 0;

            virtual void DrawIndexed(int n) = 0;
            virtual void DrawArrays(int n) = 0;


            static char* GetAPIStr();
            static API GetAPI() { return m_API; };
            static std::unique_ptr<IRendererAPI> Create();
        private:
            static API m_API;
    };

    

    // Static instance of the renderer API
    class RendererAPI {
        public:
            static void Init() {
                m_rendererAPI = IRendererAPI::Create();
                
                m_rendererAPI->Init();
            }

            static void SetViewport(uint32_t width, uint32_t height);
            static void Clear(glm::vec4 color);

            static void LoadTexture(unsigned char* data, int width, int height, bool useDefaults=true) ;
            static unsigned int GenTexture();
            static void SetTextureParameter(unsigned int param, unsigned int value);
            static void UseTexture(unsigned int id);
            static void BindTexture(unsigned int tex);

            static void CheckErrors();

            static void DrawIndexed(int n);
            static void DrawArrays(int n);

        private:
            static std::unique_ptr<IRendererAPI> m_rendererAPI;
    };
}

#endif