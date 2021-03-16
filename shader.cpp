#include <stdio.h>
#include <glad/glad.h>
#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	FILE *vertexFile;
	FILE *fragmentFile;

	vertexFilePath = vertexPath;
	fragmentFilePath = fragmentPath;

	vertexFile = fopen(vertexPath,"r");
	fragmentFile = fopen(fragmentPath, "r");
	if(vertexFile==NULL|fragmentFile==NULL){printf("Error! Couldn't open shaderfiles!\n"); return;}
	fseek(vertexFile, 0, SEEK_END);
	fseek(fragmentFile, 0, SEEK_END);
	int vertexShaderSize = ftell(vertexFile);
	int fragmentShaderSize = ftell(fragmentFile);

	fseek(vertexFile, 0, SEEK_SET);
	fseek(fragmentFile, 0, SEEK_SET);

	// Allocate shader string variable and read from shader file

	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;

	vertexShaderSource = (char*)malloc(vertexShaderSize*sizeof(char));
	fragmentShaderSource = (char*)malloc(fragmentShaderSize*sizeof(char));

	fread(vertexShaderSource, sizeof(char), vertexShaderSize,vertexFile);
	fread(fragmentShaderSource, sizeof(char), fragmentShaderSize, fragmentFile);


	fclose(vertexFile);
	fclose(fragmentFile);

	compile(vertexShaderSource, fragmentShaderSource);

	free(vertexShaderSource);
	free(fragmentShaderSource);
	
}


void Shader::compile(const char* vertexShaderSource, const char* fragmentShaderSource){
	unsigned int vertex, fragment;
	int success;
	char log[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertex,512, NULL, log); 
		printf("Error while compiling vertex shader %s! :\n %s\n", vertexFilePath, log);
		return;
	}

	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragment, 512, NULL, log);
		printf("Error while compiling fragment shader %s! :\n %s\n", fragmentFilePath, log);
		return;
	}


	ID = glCreateProgram();
	glAttachShader(ID,vertex);
	glAttachShader(ID,fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(ID, 512, NULL, log);
		printf("Error while linking shaders! : %s\n", log);
		return;
	}
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::destroy(){
	if(ID)
		glDeleteProgram(ID);
}

void Shader::use(){
	glUseProgram(ID);
}

// Uniform set functions 
void Shader::setBool(const char* name, bool value){
	glUniform1i(glGetUniformLocation(ID, name), (int)value); 
}

void Shader::setInt(const char* name, int value){
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, float value){
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setVec3(const char* name, float value1, float value2, float value3){
	glUniform3f(glGetUniformLocation(ID, name), value1, value2, value3);
}

void Shader::setVec4(const char* name, float value1, float value2, float value3, float value4){
	glUniform4f(glGetUniformLocation(ID, name), value1, value2, value3, value4);
}