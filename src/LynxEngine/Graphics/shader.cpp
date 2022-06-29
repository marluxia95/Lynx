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

#include "Platform/OpenGL/gl_shader.h"

namespace Lynx::Graphics {

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

	Shader::Shader(const std::string vertexPath, const std::string fragmentPath) : ResourceBase()
	{
		PushSource(vertexPath, SHADER_VERTEX);
		PushSource(fragmentPath, SHADER_FRAGMENT);
		Link();
	}

	/**
	 * @brief Pushes a shader source to be compiled
	 * 
	 * @param shaderPath 
	 * @param type 
	 */
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

		log_debug("SHADER DUMP \n Shader type %d \n Shader source : \n%s", obj.type, obj.source);
		obj.shader = RendererAPI::CompileShader(obj.source, obj.type);
		shader_objs.push_back(obj);
		free(obj.source);

		return success;
	}

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

		if(lpos != 0) {
			shader_source[lpos] = '\0';
		}else{
			log_error("Failed to read shader %s", path);
			free(shader_source);
			success = false;
			return NULL;
		}
		return shader_source;
	}

	/**
	 * @brief Links the shader program
	 * 
	 * @return true 
	 * @return false 
	 */
	bool Shader::Link()
	{
		program = ShaderProgram::Create();

		for( auto& shobj : shader_objs ) {
			program->AttachShader(shobj.shader);
		}

		success = program->Link();

		for( auto& shobj : shader_objs ) {
			Graphics::RendererAPI::DestroyShader(shobj.shader);
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
			int loc = Graphics::RendererAPI::GetShaderUniformLocation(program->GetID(), uniformName);
			uniform_cache_map.insert({uniformName, loc});
			return loc;
		}else{
			return uniform_cache_map[uniformName];
		}
	}

	// FIXME : Improve these templates please its giving me a headache

	template <>
	void Shader::SetUniform(const char* name, bool value)
    {
		if(!success) return;

		Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, int value)
    {
		if(!success) return;
		
        Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, float value)
    {
		if(!success) return;
		
        Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::vec2 value)
    {
		if(!success) return;
		
		Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::vec3 value)
    {
		if(!success) return;
		
        Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::vec4 value)
    {
		if(!success) return;
		
        Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::mat2 value)
    {
		if(!success) return;
		
        Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::mat3 value)
    {
		if(!success) return;
		
        Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }

	template <>
    void Shader::SetUniform(const char* name, glm::mat4 value)
    {
		if(!success) return;
		
        Graphics::RendererAPI::SetShaderUniform(getUniformLocation(name), value);
    }


}