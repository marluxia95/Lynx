#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "rendererAPI.h"

#define MAX_ERR_BUFSIZE 2048

namespace Lynx::Graphics {  

    // Abstract base class for shader program
    class ShaderProgram {
        public:
            ShaderProgram() = default;
            ~ShaderProgram() = default;
            unsigned int GetID() { return id; };
            char* GetErrorLog() { return error_log; };
            virtual void Use() = 0;
            virtual void AttachShader(unsigned int shaderID) = 0;
            virtual bool Link() = 0;

            static ShaderProgram* Create();
        protected:
            unsigned int id;
            char error_log[MAX_ERR_BUFSIZE];
    };

    class Shader
    {
        public:
            Shader(const char* vertexPath, const char* fragmentPath);
            Shader(const char* shaderFile);

            bool success = true;
            const char* shader_path = "";


            ShaderProgram* getProgram() { return program; };
            char* GetError() { return errorlog; };

            void Destroy();
            void Use();

            template<typename T>
            void SetUniform(const char* name, T value)
            {
                RendererAPI::SetShaderUniform(getUniformLocation(name), value);
            } 
            
            
            bool Reload();
        private:
            int getUniformLocation(const char* name);
            bool compile(const char* source);
            void parse(const char* raw_source);
            void loadShaderFromFile(const char* file);
            size_t shaderSize;
            char* errorlog;
            ShaderProgram* program;
            std::map<const char*, int> uniform_cache_map;
    };

}
#endif