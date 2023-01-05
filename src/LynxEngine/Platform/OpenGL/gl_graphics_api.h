#ifndef GLRENDERERAPI_H
#define GLRENDERERAPI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <GL/glew.h>
#include "Graphics/graphics_api.h"
#include "Core/logger.h"

#define API_DEBUG

GLenum checkerror_(const char* f, int l);
#ifdef __linux__
#define glCheckError() checkerror_(__PRETTY_FUNCTION__, __LINE__)
#elif _WIN32
#define glCheckError() checkerror_(__FUNCDNAME__, __LINE__)
#endif

namespace Lynx::Graphics {

    class IRendererAPI;
    enum BufferType : unsigned int;

    namespace OpenGL {

        class GLRendererAPI : public Lynx::Graphics::IRendererAPI {
        public:
            void Init();

            void SetViewport(uint32_t width, uint32_t height);
            void Clear(glm::vec4 color);

            void LoadTexture(TextureType type, unsigned char* data, int width, int height, bool useDefaults=true, int format=0x1908); // GL_RGBA
            unsigned int GenTexture();
            void SetTextureParameter(unsigned int param, unsigned int value);
            void UseTexture(unsigned int id);
            void BindTexture(unsigned int tex);
            void BindTexture(TextureType type, unsigned int tex);

            void CheckErrors(const char* f, int l);

            unsigned int CompileShader(const char* shadersource, ShaderType type);
            int GetShaderUniformLocation(unsigned int programID, const char* uniformName);
            void SetShaderUniformBool(int location, bool value);
            void SetShaderUniformInt(int location, int value);
            void SetShaderUniformFloat(int location, float value);
            void SetShaderUniformVec2(int location, glm::vec2 value);
            void SetShaderUniformVec3(int location, glm::vec3 value);
            void SetShaderUniformVec4(int location, glm::vec4 value);
            void SetShaderUniformMat2(int location, glm::mat2 value);
            void SetShaderUniformMat3(int location, glm::mat3 value);
            void SetShaderUniformMat4(int location, glm::mat4 value);
            void DestroyShader(unsigned int shaderID);

            void DrawIndexed(int n);
            void DrawArrays(int n);
        };



    }


}

#endif
