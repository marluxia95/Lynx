#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <memory>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "Core/resource.h"
#include "lynx_common.h"
#include "debug.h"
#include "rendererAPI.h"

//#define SHADER_DEBUG
#define MAX_ERR_BUFSIZE 2048

namespace Lynx {

    namespace Graphics {  


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

            static std::unique_ptr<ShaderProgram> Create();
        protected:
            unsigned int id;
            char error_log[MAX_ERR_BUFSIZE];
    };

    class LYNXENGINE_API Shader : public ResourceBase {
        private:
            class ShaderObj {
            public:
                ShaderObj(const char* path, char* source, ShaderType type) : path(path), source(source), type(type) { }
                const char* path;
                char* source;
                ShaderType type;
                unsigned int shader;
            };  

            int getUniformLocation(const char* name);
            bool compile(ShaderObj obj);
            void loadShaderFromFile(const char* file);
            char* readShaderFile(const char* file);

        public:
            Shader(std::string vertexPath, std::string fragmentPath);
            Shader();

            void PushSource(std::string shaderPath, ShaderType type);

            void Use();

            bool Link();

            template<typename T>
            void SetUniform(const char* name, T value);
            
            bool Reload();
            char* GetError() { return errorlog; }

            int success = true;
        private:
            size_t shaderSize;
            char* errorlog;

            std::unique_ptr<ShaderProgram> program;

            std::map<const char*, int> uniform_cache_map;

            std::vector<ShaderObj> shader_objs;
    };

    }

}

#endif