#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h> 
#include <map>

#define MAX_ERR_BUFSIZE 2048

namespace Lynx::Graphics {  

    class Shader
    {
        public:
            Shader(const char* vertexPath, const char* fragmentPath);
            Shader(const char* shaderFile);

            // Shader variables with initialized values
            unsigned int ID = 0;
            bool success = true;
            const char* name = "";
            const char* vertexFilePath = "";
            const char* fragmentFilePath = "";


            GLuint getProgram();
            char* getError();

            void destroy();
            void use();
            void setBool(const char* name, bool value);  
            void setInt(const char* name, int value);   
            void setFloat(const char* name, float value);
            void setVec3(const char* name, float value1, float value2, float value3);
            void setVec3(const char* name, const glm::vec3 &value);
            void setVec4(const char* name, float value1, float value2, float value3, float value4);
            void setMat4(const char* name, const glm::mat4 &value);
            bool Reload();
        private:
            bool compile(const char* vertexShaderSource, const char* fragmentShaderSource);
            void loadShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);
            char errorlog[MAX_ERR_BUFSIZE];
    };

}
#endif