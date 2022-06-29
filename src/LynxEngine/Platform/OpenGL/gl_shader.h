#ifndef GLSHADER_H
#define GLSHADER_H

#include "Graphics/shader.h"

namespace Lynx::Graphics::OpenGL {

class GLShaderProgram : public Lynx::Graphics::ShaderProgram {
    public:
        GLShaderProgram();
        ~GLShaderProgram();
        void Use();
        void AttachShader(unsigned int shaderID);
        bool Link();
};

}

#endif