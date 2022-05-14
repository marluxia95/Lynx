/**
 * @file shader.cpp
 * @author marlxx
 * @brief A shader class wrapper that parses, compiles and manages a shader
 * @version 0.0.3
 * @date 2021-09-03
 * 
 * @copyright Copyright (c) 2021. See LICENSE for more information
 * 
 */

#include <stdio.h>
#include <string>
#include <sstream>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "Core/logger.h"
#include "Core/assert.h"
#include "Utils/path.hpp"
#include "debug.h"

#include "Platform/OpenGL/GLShader.h"

namespace Lynx::Graphics {

	/*
	ShaderProgram::ShaderProgram()
    {
        id = glCreateProgram(); 
        log_debug("[GL] Initialized shader program with ID %d", id);
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(id);
        log_debug("[GL] Deleted shader program with ID %d", id);
    }

    void ShaderProgram::AttachShader(unsigned int shaderID)
    {
        glAttachShader(id, shaderID);
        log_debug("[GL] Attached shader program with ID %d to %d", shaderID, id);
    }

    void ShaderProgram::Use()
    {
        glUseProgram(id);
    }
    
    bool ShaderProgram::Link()
    {
        glLinkProgram(id);
		int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(id, MAX_ERR_BUFSIZE, NULL, error_log);
			log_error("Error while linking shaders! : %s", error_log);
		}
		glValidateProgram(id);
		glCheckError();
        log_debug("[GL] Linked shader program with ID %d; Success : %d", id, success);
		return (bool)success;
    }*/

	std::unique_ptr<ShaderProgram> ShaderProgram::Create()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<OpenGL::GLShaderProgram>();
			default : return nullptr;
		}
	}

	Shader::Shader() : ResourceBase()
	{
		
	}

	Shader::Shader(std::string vertexPath, std::string fragmentPath) : ResourceBase()
	{
		PushSource(vertexPath, SHADER_VERTEX);
		PushSource(fragmentPath, SHADER_FRAGMENT);
		Link();
	}

	void Shader::PushSource(std::string shaderPath, ShaderType type)
	{
		const char* shader_path_str = shaderPath.c_str();
		char* shader_source = readShaderFile(shader_path_str);
		ShaderObj obj = ShaderObj(shader_path_str, shader_source, type);

		log_debug("Compiling shader %s", shader_path_str);
		compile(obj);
	}

	bool Shader::compile(ShaderObj obj)
	{
		if(!obj.source)
			return false;

		

		log_debug("Creating shader object");
		/*obj.shader = glCreateShader(obj.type);
		glShaderSource(obj.shader, 1, &obj.source, NULL);

		glCompileShader(obj.shader);
		glGetShaderiv(obj.shader, GL_COMPILE_STATUS, &success);
		log_debug("Compiled shader object %d", success);
		if(!success){
			errorlog = (char*)malloc(512);
			glGetShaderInfoLog(obj.shader, 512, NULL, errorlog); 
			log_error("Error while compiling shader %s! :\n\t%s", obj.path, errorlog);
			free(errorlog);
			return false;
		}

		log_debug("Pushing shader object");*/

		log_debug("SHADER DUMP \n Shader type %d \n Shader source : \n%s", obj.type, obj.source);
		obj.shader = RendererAPI::CompileShader(obj.source, obj.type);
		shader_objs.push_back(obj);

		return success;
	}

	/*
	std::string Shader::parse(ShaderObj obj) {
		std::istringstream input(obj.source);
		std::string result;
		std::string line;
		
		while ( std::getline(input, line) ) {
			int ismacro = EOF;
			char macrostr[64];
			char macrocontents[64];

			ismacro = sscanf(line.c_str(), "#%s %s", &macrostr, &macrocontents);

			if(ismacro == EOF){
				result.append(line + '\n');
				continue;
			}

			if(macrostr == "include") {
				std::string shader_path = Utils::GetAbsolutePath(obj.path, macrocontents);
				printf("%s", shader_path.c_str());
				std::string included_source = std::string(readShaderFile(shader_path.c_str()));
				result.append(included_source);
			}

			
		}
		puts(result.c_str());
		return result;
	}*/

	char* Shader::readShaderFile(const char* path) {
		FILE *shader_file;
		log_debug("Loading shader %s", path);

		shader_file = fopen(path,"r");
		if(!shader_file){
			log_error("Unable to open shader %s", path); 
			success = false;
			return NULL;
		}

		fseek(shader_file, 0, SEEK_END);
		size_t total_shader_size = ftell(shader_file);
		shaderSize = total_shader_size;

		rewind( shader_file );

		char* shader_source = (char*)malloc(total_shader_size+1); // Prevent buffering errors

		int lpos = fread(shader_source, sizeof(char), total_shader_size, shader_file);

		fclose(shader_file);

		if(lpos != 0 | shader_source != NULL) {
			shader_source[lpos] = '\0';
		}else{
			log_error("Failed to read shader %s", path);
			free(shader_source);
			success = false;
			return NULL;
		}
		return shader_source;
	}

	bool Shader::Link()
	{
		program = ShaderProgram::Create();

		for( auto& shobj : shader_objs ) {
			program->AttachShader(shobj.shader);
		}

		success = program->Link();

		for( auto& shobj : shader_objs ) {
			glDeleteShader(shobj.shader);
		}
	}

	/**
	 * @brief Reloads the shader
	 * 
	 * @return bool True if shader was compiled successfully, false if there was an error while compiling the shader
	 */
	bool Shader::Reload(){
		LYNX_ASSERT(program, "Invalid shader");

		return success;
	}

	/**
	 * @brief Uses the shader's program
	 * 
	 */
	void Shader::Use(){
		if(!success|!program) return;
		
		program->Use();
	}

	int Shader::getUniformLocation(const char* uniformName)
	{
		if(uniform_cache_map.find(uniformName) == uniform_cache_map.end()) {
			int loc = glGetUniformLocation(program->GetID(), uniformName);
			uniform_cache_map.insert({uniformName, loc});
			return loc;
		}else{
			return uniform_cache_map[uniformName];
		}
	}

	template <>
	void Shader::SetUniform(const char* name, bool value)
    {
		if(!success) return;

        glUniform1i(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, int value)
    {
		if(!success) return;
		
        glUniform1i(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, float value)
    {
		if(!success) return;
		
        glUniform1f(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::vec2 value)
    {
		if(!success) return;
		
        glUniform2fv(getUniformLocation(name), 1, &value[0]);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::vec3 value)
    {
		if(!success) return;
		
        glUniform3fv(getUniformLocation(name), 1, &value[0]);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::vec4 value)
    {
		if(!success) return;
		
        glUniform4fv(getUniformLocation(name), 1, &value[0]);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::mat2 value)
    {
		if(!success) return;
		
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

	template <>
    void Shader::SetUniform(const char* name, glm::mat3 value)
    {
		if(!success) return;
		
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

	template <>
    void Shader::SetUniform(const char* name, glm::mat4 value)
    {
		if(!success) return;
		
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }


}