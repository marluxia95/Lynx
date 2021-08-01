#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "GLRendererAPI.h"

namespace Lynx::Graphics::OpenGL {

    void GLRendererAPI::Init()
    {
        bool err = glewInit() != GLEW_OK;   

        if(err){
            log_fatal("Failed to initalize GLEW");
            exit(1);
        }

        log_info("Sucessfully loaded API_OPENGL");
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

    void GLRendererAPI::RenderIndexed() 
    {
        glDrawElements()
    }

    GLRendererAPI::~RendererAPI()
    {

    }


}