#include <stdio.h>
#include <string.h>

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
	if(shaderFile == NULL){printf("Error ! Couldn't open shaderfile");}
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
		        	printf("Found vertex shader %d\n", vertexShader);
		        }else if(fragmentHeader != NULL){
		        	currentShader = 2;
		        	printf("Found fragment shader %d\n", fragmentShader);
		        }
		        break;
		    case 1:
		    	if(fragmentHeader != NULL){
		    		printf("Found fragment shader %d\n", fragmentShader);
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
		    		printf("Found vertex shader %d\n", vertexShader);
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
    compile(vertexShader, fragmentShader);

    free(line);
    free(vertexShader);
    free(fragmentShader);


}

void Shader::compile(const char* vertexShaderSource, const char* fragmentShaderSource){
	unsigned int vertex, fragment;
	int success;
	char log[512];
	
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
		glGetShaderInfoLog(vertex,512, NULL, log); 
		printf("Error while compiling vertex shader %s! :\n %s\n", vertexFilePath, log);
		return;
	}

	printf("Compiling fragment shader ...\n");
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragment, 512, NULL, log);
		printf("Error while compiling fragment shader %s! :\n %s\n", fragmentFilePath, log);
		return;
	}

	printf("Linking...\n");
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
	printf("Successfully compiled shader, cleaning up...\n");
	
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