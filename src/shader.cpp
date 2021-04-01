#include <stdio.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include "shader.h"

namespace Lynx {

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	FILE *vertexFile;
	FILE *fragmentFile;

	vertexFilePath = vertexPath;
	fragmentFilePath = fragmentPath;

	vertexFile = fopen(vertexPath,"r");
	fragmentFile = fopen(fragmentPath, "r");
	if(!vertexFile|!fragmentFile){printf("Error! Couldn't open shaderfiles!\n"); return;}
	fseek(vertexFile, 0, SEEK_END);
	fseek(fragmentFile, 0, SEEK_END);
	int vertexShaderSize = ftell(vertexFile);
	int fragmentShaderSize = ftell(fragmentFile);

	rewind( vertexFile );
	rewind( fragmentFile );

	// Allocate shader string variable and read from shader file

	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;

	vertexShaderSource = (char*)malloc(1+vertexShaderSize);
	fragmentShaderSource = (char*)malloc(1+fragmentShaderSize);

	fread(vertexShaderSource, sizeof(char), vertexShaderSize,vertexFile);
	fread(fragmentShaderSource, sizeof(char), fragmentShaderSize, fragmentFile);

	fclose(vertexFile);
	fclose(fragmentFile);

	vertexShaderSource[vertexShaderSize] = '\0';
	fragmentShaderSource[fragmentShaderSize] = '\0';

	compile(vertexShaderSource, fragmentShaderSource);

	free(vertexShaderSource);
	free(fragmentShaderSource);
	
}

Shader::Shader(const char* shaderPath){
	FILE *shaderFile;

	shaderFile = fopen(shaderPath, "r");
	if(shaderFile == NULL){printf("Error ! Couldn't open shaderfile"); return;}
	fseek(shaderFile, 0, SEEK_END);
	int fileSize = ftell(shaderFile);
	rewind(shaderFile);

	char *shaderSource = NULL;
	shaderSource = (char*)malloc(fileSize*sizeof(char));
	fread(shaderSource, sizeof(char), fileSize, shaderFile);

	rewind(shaderFile);
	

	// Remove EOF 
	shaderSource[strcspn(shaderSource, "\n")] = 0;

	char *line = NULL;
	char *vertexHeader;
	char *fragmentHeader;
	char *endHeader;
	char *parsingBuffer = NULL;

	parsingBuffer = (char*)malloc(fileSize*sizeof(char));
	parsingBuffer[0] = '\0';

	char *vertexShader = NULL;
	char *fragmentShader = NULL;

	int currentShader;
	int shaderSize;
	size_t len;
	ssize_t read;
	printf("Shader type   | size | content\n");
	while ((read = getline(&line, &len, shaderFile)) != -1) {
		vertexHeader = strstr(line, "#shader vertex");
		fragmentHeader = strstr(line, "#shader fragment");
		switch(currentShader){
			
			case 0:
				if(vertexHeader != NULL){
		        	currentShader = 1;
		        }else if(fragmentHeader != NULL){
		        	currentShader = 2;
		        }
		        break;
		    case 1:
		    	if(fragmentHeader != NULL){
		    		printf("Starting to allocate %d from vertex shader ...\n", shaderSize);
		    		vertexShader = (char*)malloc(shaderSize);
		    		printf("Starting to copy data from buffer ...\n");
		    		memcpy(&vertexShader,&parsingBuffer, shaderSize);
		    		currentShader = 2;
		    		break;
		    	}else{
		    		shaderSize += sizeof(line);
		    		printf("Vertex Shader |  %d  | %s",sizeof(line),line);
		    		strcat(parsingBuffer, line);
		    	}
		    	break;
		    case 2:
		    	printf("Beggining of fragment shader \n");
		    	if(vertexHeader != NULL){
		    		printf("Starting to allocate %d from fragment shader ...\n", shaderSize);
		    		fragmentShader = (char*)malloc(shaderSize);
		    		printf("Starting to copy data from buffer ...\n");
		    		memcpy(&fragmentShader,&parsingBuffer, shaderSize);
		    		printf("Cleaning up buffer ...\n");
		    		parsingBuffer = "\0";
		    		shaderSize = 0;
		    		currentShader = 0;
		    		break;
		    	}else{
		    		printf("Fragment Shader |  %d  | %s",sizeof(line),line);
		    		shaderSize += sizeof(line);
		    		strcat(parsingBuffer, line);
		    	}
		    	break;

		}

        
    }
	

    fclose(shaderFile);
    free(shaderSource);	

    printf(fragmentShader);
    success = compile(vertexShader, fragmentShader);

    free(line);
    free(vertexShader);
    free(fragmentShader);


}

bool Shader::compile(const char* vertexShaderSource, const char* fragmentShaderSource){
	unsigned int vertex, fragment;
	int success;
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	printf("Getting shader sources ...\n");
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	printf("Starting to compile shaders ...\n");
	printf("Compiling vertex shader ...\n");
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertex,512, NULL, errorlog); 
		printf("Error while compiling vertex shader %s! :\n %s\n", vertexFilePath, errorlog);
		return false;
	}

	printf("Compiling fragment shader ...\n");
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragment, 512, NULL, errorlog);
		printf("Error while compiling fragment shader %s! :\n %s\n", fragmentFilePath, errorlog);
		return false;
	}

	printf("Linking and creating program...\n");
	this->ID = glCreateProgram();
	glAttachShader(this->ID,vertex);
	glAttachShader(this->ID,fragment);
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(ID, 512, NULL, errorlog);
		printf("Error while linking shaders! : %s\n", errorlog);
		return false;
	}
	printf("Successfully compiled shader ID : %d , cleaning up...\n", this->ID);
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
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