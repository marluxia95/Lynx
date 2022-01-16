#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>
#include "Core/logger.h"

GLenum gl_checkerror(const char* f, int l)
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
        log_error("GLerror in %s ( %d ) : %s", f, l, error);
    }
    return errorCode;
}

#ifdef __linux__
#define glCheckError() gl_checkerror(__PRETTY_FUNCTION__, __LINE__)
#elif _WIN32
#define glCheckError() gl_checkerror(__FUNCDNAME__, __LINE__)
#endif

#endif