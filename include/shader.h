#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <GL/glew.h> 
#include <stdio.h>
#include "lights.h"


namespace Lynx {  

class Shader
{
    public:
        // the program ID
        unsigned int ID;
      

        Shader(const char* vertexPath, const char* fragmentPath);
        Shader(const char* shaderFile);

        GLuint getProgram();

        char* getError();

        void destroy();
        void use();
        bool success = true;

        void setBool(const char* name, bool value);  
        void setInt(const char* name, int value);   
        void setFloat(const char* name, float value);
        void setVec3(const char* name, float value1, float value2, float value3);
        void setVec3(const char* name, const glm::vec3 &value);
        void setVec4(const char* name, float value1, float value2, float value3, float value4);
        void setMat4(const char* name, const glm::mat4 &value);
    private:
        bool compile(const char* vertexShaderSource, const char* fragmentShaderSource);
        const char* vertexFilePath;
        const char* fragmentFilePath;
        char errorlog[512];
};

}
#endif