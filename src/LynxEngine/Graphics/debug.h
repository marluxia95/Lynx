#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>
#include "Core/logger.h"

inline const char* gl_translate_error(GLenum err) {
    const char* error;
    switch (err)
    {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }
    return error;
} 

inline GLenum gl_checkerror(const char* f, int l, int s=NULL)
{
    GLenum errorCode = 0;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        if(s)
            break;

        log_error("GLerror in %s ( %d ) : %s", f, l, gl_translate_error(errorCode));
    }
    return errorCode;
}

#ifdef __linux__
#define glCheckError() gl_checkerror(__PRETTY_FUNCTION__, __LINE__)
#elif _WIN32
#define glCheckError() gl_checkerror(__FUNCDNAME__, __LINE__)
#endif

#endif