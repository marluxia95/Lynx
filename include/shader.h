
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <stdio.h>
#include <fstream>
#include <iostream>
  

class Shader
{
    public:
        // the program ID
        unsigned int ID;
      

        Shader(const char* vertexPath, const char* fragmentPath);
        Shader(const char* shaderFile);
        void destroy();
        void use();

        void setBool(const char* name, bool value);  
        void setInt(const char* name, int value);   
        void setFloat(const char* name, float value);
        void setVec3(const char* name, float value1, float value2, float value3);
        void setVec4(const char* name, float value1, float value2, float value3, float value4);
    private:
        void compile(const char* vertexShaderSource, const char* fragmentShaderSource);
        const char* vertexFilePath;
        const char* fragmentFilePath;
};
  
#endif