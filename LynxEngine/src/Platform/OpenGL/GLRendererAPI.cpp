#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <cassert>
#include "GLRendererAPI.h"
#include "Core/logger.h"

namespace Lynx::Graphics::OpenGL {

    void GLRendererAPI::Init()
    {
        bool err = glewInit() != GLEW_OK;   

        if(err){
            log_fatal("Failed to initalize GLEW");
            exit(1);
        }

        glEnable(GL_DEPTH_TEST);

        // Enable face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glFrontFace(GL_CW);  

        log_info("Loaded OpenGL API");
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

    void GLRendererAPI::GenBuffers(uint* bufferObj)
    {
        glGenBuffers(1, bufferObj);
    }

    void GLRendererAPI::BindBuffer(BufferType bufferType, uint* bufferobj)
    {
        switch( bufferType )
        {
            case FRAMEBUF:
                glBindBuffer(GL_FRAMEBUFFER, *bufferobj);
                break;
                
            default:
                glBindBuffer(GL_ARRAY_BUFFER, *bufferobj);
                break;
        }   
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
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void GLRendererAPI::SetTextureParameter(unsigned int param, unsigned int value)
    {
        glTexParameteri(GL_TEXTURE_2D, param, value);
    }

    void GLRendererAPI::UseTexture(unsigned int id)
    {
        glActiveTexture(GL_TEXTURE0 + id);
    }

    void GLRendererAPI::BindTexture(unsigned int tex)
    {
        glBindTexture(GL_TEXTURE_2D, tex);
    }

    void GLRendererAPI::RenderIndexed(int n)
    {
        glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, (void*)0);
    }

}