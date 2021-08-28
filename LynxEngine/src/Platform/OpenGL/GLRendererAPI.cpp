#include <glm/glm.hpp>
#include <cassert>
#include <GL/glew.h>
#include "GLRendererAPI.h"
#include "Core/logger.h"

GLenum gl_checkerror_(const char* f, int l)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        log_error("%s ( %d ) : %s", f, l, error);
    }
    return errorCode;
}

namespace Lynx::Graphics::OpenGL {

    void GLRendererAPI::Init()
    {
        if (glewInit() != GLEW_OK)
        {
            log_fatal("GLRendererAPI: Unable to load GLEW");
            exit(-1);
        }

        glEnable(GL_DEPTH_TEST);

        // Enable face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glFrontFace(GL_CW);  

        log_info("GLRendererAPI: Loaded API ( %s )", glGetString(GL_VERSION));
    }

    void GLRendererAPI::Clear(glm::vec4 color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLRendererAPI::SetViewport(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);
    }
    
    unsigned int GLRendererAPI::GenTexture()
    {
        unsigned int tex;
        glGenTextures(1, &tex);
        return tex;
    }

    void GLRendererAPI::LoadTexture(unsigned char* data, int width, int height, bool useDefaults)
    {
        assert(data != NULL && "GL : Invalid data");
        if(useDefaults) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glCheckError();
    }

    void GLRendererAPI::SetTextureParameter(unsigned int param, unsigned int value)
    {
        glTexParameteri(GL_TEXTURE_2D, param, value);
        glCheckError();
    }

    void GLRendererAPI::UseTexture(unsigned int id)
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glCheckError();
    }

    void GLRendererAPI::BindTexture(unsigned int tex)
    {
        glBindTexture(GL_TEXTURE_2D, tex);
    }

    void GLRendererAPI::DrawIndexed(int n)
    {
        glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, (void*)0);
        glCheckError();
    }

    void GLRendererAPI::DrawArrays(int n)
    {
        glDrawArrays(GL_TRIANGLES, 0, n);
        glCheckError();
    }

    void GLRendererAPI::CheckErrors()
    {
        glCheckError();
    }
}
