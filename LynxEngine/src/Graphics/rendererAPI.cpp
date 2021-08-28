#include "rendererAPI.h"
#include "Platform/OpenGL/GLRendererAPI.h"

namespace Lynx::Graphics {

    char* API_str[] = {
        "OpenGL"
    };

    API IRendererAPI::m_API = API_OPENGL;
    std::unique_ptr<IRendererAPI> RendererAPI::m_rendererAPI = nullptr;
    
    std::unique_ptr<IRendererAPI> IRendererAPI::Create()
    {
        switch(m_API) {
            case API_OPENGL : 
                return std::make_unique<OpenGL::GLRendererAPI>();
        }
    }

    char* IRendererAPI::GetAPIStr()
    {
        return API_str[(unsigned int)m_API];
    }

    void RendererAPI::SetViewport(uint32_t width, uint32_t height)
    {
        m_rendererAPI->SetViewport(width, height);
    }

    void RendererAPI::Clear(glm::vec4 color) 
    { 
        m_rendererAPI->Clear(color); 
    }

    void RendererAPI::LoadTexture(unsigned char* data, int width, int height, bool useDefaults) 
    { 
        m_rendererAPI->LoadTexture(data, width, height); 
    }

    unsigned int RendererAPI::GenTexture() 
    { 
        return m_rendererAPI->GenTexture();
    }

    void RendererAPI::SetTextureParameter(unsigned int param, unsigned int value) 
    { 
        m_rendererAPI->SetTextureParameter(param, value); 
    }
    void RendererAPI::UseTexture(unsigned int id) 
    { 
        m_rendererAPI->UseTexture(id); 
    }

    void RendererAPI::BindTexture(unsigned int tex) 
    { 
        m_rendererAPI->BindTexture(tex); 
    }

    void RendererAPI::DrawIndexed(int n) 
    { 
        m_rendererAPI->DrawIndexed(n); 
    }

    void RendererAPI::DrawArrays(int n) 
    { 
        m_rendererAPI->DrawArrays(n); 
    }

    void RendererAPI::CheckErrors()
    {
        m_rendererAPI->CheckErrors();
    }


}
