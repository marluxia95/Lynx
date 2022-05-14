#include <GL/glew.h>
#include "Graphics/rendererAPI.h"
#include "Core/logger.h"
#include "GLRendererAPI.h"
#include "GLShader.h"

namespace Lynx::Graphics::OpenGL {

    GLShaderProgram::GLShaderProgram()
    {
        id = glCreateProgram(); 
#ifdef API_DEBUG
        log_debug("[GL] Initialized shader program with ID %d", id);
#endif
    }

    GLShaderProgram::~GLShaderProgram()
    {
        glDeleteProgram(id);
#ifdef API_DEBUG
        log_debug("[GL] Deleted shader program with ID %d", id);
#endif
    }

    void GLShaderProgram::AttachShader(unsigned int shaderID)
    {
        glAttachShader(id, shaderID);
#ifdef API_DEBUG
        log_debug("[GL] Attached shader program with ID %d to %d", shaderID, id);
#endif
    }

    void GLShaderProgram::Use()
    {
        glUseProgram(id);
#ifdef API_DEBUG
        log_debug("[GL] Used shader program with ID %d", id);
#endif
    }
    
    bool GLShaderProgram::Link()
    {
        glLinkProgram(id);
		int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(id, MAX_ERR_BUFSIZE, NULL, error_log);
			log_error("Error while linking shaders! : %s", error_log);
		}
#ifdef API_DEBUG
        log_debug("[GL] Linked shader program with ID %d; Success : %d", id, success);
#endif
        glCheckError();
		return (bool)success;
    }
}