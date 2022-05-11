#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLRendererAPI.h"
#include "Graphics/shader.h"
#include "Core/logger.h"
#include "Core/assert.h"

GLenum checkerror_(const char* f, int l)
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

namespace Lynx::Graphics::OpenGL {

    std::map<ShaderType, GLenum> shader_type_map = {
        {SHADER_NULL,       NULL                },
        {SHADER_VERTEX,     GL_VERTEX_SHADER    },
        {SHADER_FRAGMENT,   GL_FRAGMENT_SHADER  },
        {SHADER_GEOMETRY,   GL_GEOMETRY_SHADER  },
        {SHADER_COMPUTE,    GL_COMPUTE_SHADER   }
    };

    std::map<TextureType, GLenum> texture_type_map = {
        {TEXTURE_1D,                GL_TEXTURE_1D},
        {TEXTURE_1D_ARRAY,          GL_TEXTURE_1D_ARRAY},
        {TEXTURE_2D,                GL_TEXTURE_2D},
        {TEXTURE_2D_ARRAY,          GL_TEXTURE_2D_ARRAY},
        {TEXTURE_3D,                GL_TEXTURE_3D},
        {TEXTURE_CUBE_MAP,          GL_TEXTURE_CUBE_MAP},
        {TEXTURE_CUBE_MAP_ARRAY,    GL_TEXTURE_CUBE_MAP_ARRAY}
    };

    void GLRendererAPI::Init()
    {
        glewExperimental = true;
        LYNX_ASSERT(glewInit() == GLEW_OK, "Unable to initialize GLEW");

        glEnable(GL_DEPTH_TEST);

        // Enable face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glFrontFace(GL_CW);  

        log_info("GLRendererAPI: Loaded API ( %s )", glGetString(GL_VERSION));
        glCheckError();
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
        glCheckError();
        return tex;
    }

    void GLRendererAPI::LoadTexture(TextureType type, unsigned char* data, int width, int height, bool useDefaults, int format)
    {
        LYNX_ASSERT(data != NULL, "Invalid data");
        unsigned int t_type = texture_type_map[type];
        if(useDefaults) {
            glTexParameteri(t_type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
            glTexParameteri(t_type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(t_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(t_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        
        glTexImage2D(t_type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(t_type);
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

    void GLRendererAPI::BindTexture(TextureType type, unsigned int tex)
    {
        glBindTexture(texture_type_map[type], tex);
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

    void GLRendererAPI::CheckErrors(const char* f, int l)
    {
        checkerror_(f,l);
    }

    unsigned int GLRendererAPI::CompileShader(const char* shaderSource, ShaderType type)
    {
        unsigned int shaderID = glCreateShader(shader_type_map[type]);
        glShaderSource(shaderID, 1, &shaderSource, NULL);
        glCompileShader(shaderID);
        glCheckError();

        // Check shader errors
        int success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shaderID, MAX_ERR_BUFSIZE, NULL, error_log);
            log_error("Error while compiling shader! : %s", error_log);
            return 0;
        }
        return shaderID;
    }

    void GLRendererAPI::SetShaderUniformBool(int location, bool value)
    {
        glUniform1i(location, value);
    }

    void GLRendererAPI::SetShaderUniformInt(int location, int value)
    {
        glUniform1i(location, value);
    }

    void GLRendererAPI::SetShaderUniformFloat(int location, float value)
    {
        glUniform1f(location, value);
    }

    void GLRendererAPI::SetShaderUniformVec2(int location, glm::vec2 value)
    {
        glUniform2fv(location, 1, &value[0]);
    }

    void GLRendererAPI::SetShaderUniformVec3(int location, glm::vec3 value)
    {
        glUniform3fv(location, 1, &value[0]);
    }

    void GLRendererAPI::SetShaderUniformVec4(int location, glm::vec4 value)
    {
        glUniform4fv(location, 1, &value[0]);
    }

    void GLRendererAPI::SetShaderUniformMat2(int location, glm::mat2 value)
    {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLRendererAPI::SetShaderUniformMat3(int location, glm::mat3 value)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLRendererAPI::SetShaderUniformMat4(int location, glm::mat4 value)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    
    void GLRendererAPI::DestroyShader(unsigned int shaderID)
    {
        glDeleteShader(shaderID);
    }

    int GLRendererAPI::GetShaderUniformLocation(unsigned int programID, const char* uniformName)
    {
        return glGetUniformLocation(programID, uniformName);
    }

}
