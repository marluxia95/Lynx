#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <memory>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <GL/glew.h> 
#include "Core/resource.h"
#include "lynx_common.h"

//#define SHADER_DEBUG
#define MAX_ERR_BUFSIZE 2048

namespace Lynx {

    namespace Graphics {  


    // Abstract base class for shader program
    class ShaderProgram {
        public:
            ShaderProgram();
            ~ShaderProgram();
            unsigned int GetID() { return id; };
            char* GetErrorLog() { return error_log; };
            void Use();
            void AttachShader(unsigned int shaderID);
            bool Link();

            static ShaderProgram* Create();
        protected:
            unsigned int id;
            char error_log[MAX_ERR_BUFSIZE];
    };

    class LYNXENGINE_API Shader : public ResourceBase {
        private:
            class ShaderObj {
            public:
                ShaderObj(const char* path, const char* source, GLenum type) : path(path), source(source), type(type) { }
                const char* path;
                const char* source;
                GLenum type;
                GLuint shader;
            };  

            int getUniformLocation(const char* name);
            bool compile(ShaderObj obj);
            void parse(const char* raw_source);
            void loadShaderFromFile(const char* file);
        public:
            Shader(std::string vertexPath, std::string fragmentPath);
            Shader();

            void PushSource(std::string shaderPath, GLenum type);

            void Use();

            bool Link();

            template<typename T>
            void SetUniform(const char* name, T value);
            
            bool Reload();
            char* GetError() { return errorlog; };

            bool success = true;
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