#include <stdio.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include "shader.h"
#include "logger.h"

namespace Lynx {

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	
	loadShaderFromFile(vertexPath, fragmentPath);
}

Shader::Shader(const char* shaderPath){
	FILE *shaderFile;

	char* buffer = 0;

	shaderFile = fopen(shaderPath, "r");
	if(shaderFile == NULL){log_error("Unable to open shaderfile %s", shaderPath); return;}
	fseek(shaderFile, 0, SEEK_END);
	int fileSize = ftell(shaderFile);
	rewind(shaderFile);

	buffer = (char*)malloc(fileSize + 1);

	fread(buffer, sizeof(char), fileSize, shaderFile);

	char* vertexShaderSource, *fragmentShaderSource;
	
	printf("%s\n",buffer);

	char* line;
	char* vertexShaderChunk = "", *fragmentShaderChunk = "";
	/*
	// Get the vertex shader chunk
	while ( (line = strsep(&buffer, "\n") ) != NULL  ) {
		char* header;
		if( (header = strstr(line, "#shader vertex")) != NULL ) { 
			vertexShaderChunk = header;
			printf("Found vertex header\n");
		}
	}

	// Get the start pointer of the fragment chunk
	while ( (line = strsep(&buffer, "\n") ) != NULL  ) {
		char* header;
		if( (header = strstr(line, "#shader fragment") ) != NULL ) {
			fragmentShaderChunk = header;
			printf("Found fragment header\n");
		}

	}
	*/
	printf("Vertex Position \n %td \n", vertexShaderChunk - buffer);
	printf("Fragment Position \n %td \n", fragmentShaderChunk - buffer);

	fclose(shaderFile);
	free(buffer);

}

void Shader::loadShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath) {
	FILE *vertexFile;
	FILE *fragmentFile;

	vertexFilePath = vertexShaderPath;
	fragmentFilePath = fragmentShaderPath;

	vertexFile = fopen(vertexFilePath,"r");
	fragmentFile = fopen(fragmentFilePath, "r");
	if(!vertexFile|!fragmentFile){log_error("Unable to open shaderfiles %s %s", vertexFilePath, fragmentFilePath); return;}
	fseek(vertexFile, 0, SEEK_END);
	fseek(fragmentFile, 0, SEEK_END);
	int vertexShaderSize = ftell(vertexFile);
	int fragmentShaderSize = ftell(fragmentFile);

	rewind( vertexFile );
	rewind( fragmentFile );

	// Allocate shader string variable and read from shader file

	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;

	vertexShaderSource = (char*)malloc(vertexShaderSize);
	fragmentShaderSource = (char*)malloc(fragmentShaderSize);

	int vspos = fread(vertexShaderSource, sizeof(char), vertexShaderSize,vertexFile);
	int fspos = fread(fragmentShaderSource, sizeof(char), fragmentShaderSize, fragmentFile);

	fclose(vertexFile);
	fclose(fragmentFile);
	

	if(vspos != 0|vertexShaderSource!=NULL){
		vertexShaderSource[vspos] = '\0';
	}
	if(fspos != 0|fragmentShaderSource!=NULL){
		fragmentShaderSource[fspos] = '\0';
	}
	

	printf("%s\n", fragmentShaderSource);

	success = compile(vertexShaderSource, fragmentShaderSource);

	free(vertexShaderSource);
	free(fragmentShaderSource);
}

bool Shader::compile(const char* vertexShaderSource, const char* fragmentShaderSource){
	unsigned int vertex, fragment;
	int success;

	if(ID){
		destroy();
	}
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	log_debug("Getting shader sources ...");
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	log_debug("Starting to compile shaders ...");
	log_debug("Compiling vertex shader ...");
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertex,512, NULL, errorlog); 
		printf("Error while compiling vertex shader %s! :\n %s", vertexFilePath, errorlog);
		return false;
	}

	log_debug("Compiling fragment shader ...");
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragment, 512, NULL, errorlog);
		log_error("Error while compiling fragment shader %s! :\n %s", fragmentFilePath, errorlog);
		return false;
	}

	log_debug("Linking and creating program...");
	this->ID = glCreateProgram();
	glAttachShader(this->ID,vertex);
	glAttachShader(this->ID,fragment);
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(ID, 512, NULL, errorlog);
		log_error("Error while linking shaders! : %s", errorlog);
		return false;
	}
	log_debug("Successfully compiled shader ID : %d , cleaning up...", this->ID);
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;

	
}

bool Shader::Reload(){
	assert(vertexFilePath && fragmentFilePath && ID);

	loadShaderFromFile(vertexFilePath, fragmentFilePath);

	return success;
}

char* Shader::getError(){
	return errorlog;
}

void Shader::destroy(){
	if(ID)
		glDeleteProgram(this->ID);
}

void Shader::use(){
	glUseProgram(this->ID);
}

GLuint Shader::getProgram(){
	return this->ID;
}

// Uniform set functions 
void Shader::setBool(const char* name, bool value){
	glUniform1i(glGetUniformLocation(this->ID, name), (int)value); 
}

void Shader::setInt(const char* name, int value){
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::setFloat(const char* name, float value){
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::setVec3(const char* name, float value1, float value2, float value3){
	glUniform3f(glGetUniformLocation(this->ID, name), value1, value2, value3);
}

void Shader::setVec3(const char* name, const glm::vec3 &value)
{ 
	glUniform3fv(glGetUniformLocation(this->ID, name), 1, &value[0]); 
}

void Shader::setVec4(const char* name, float value1, float value2, float value3, float value4){
	glUniform4f(glGetUniformLocation(this->ID, name), value1, value2, value3, value4);
}

void Shader::setMat4(const char* name, const glm::mat4 &value){
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(value));
}


}