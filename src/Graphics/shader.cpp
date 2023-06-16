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

#include "gl_graphics_api.h"

namespace Lynx {

	std::unique_ptr<ShaderProgram> ShaderProgram::Create()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return std::make_unique<GLShaderProgram>();
			default : return nullptr;
		}
	}

	Shader::Shader() : Resource()
	{
	}

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) : Resource(), shaderSize(0)
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
	void Shader::PushSource(const std::string& shaderPath, ShaderType type)
	{
		const char* shader_path_str = shaderPath.c_str();
		char* shader_source = readShaderFile(shader_path_str);
		ShaderObj obj = ShaderObj(shader_path_str, shader_source, type);
		if(!success)
			return;

		log_debug("Compiling shader %s", shader_path_str);
		compile(obj);
	}

	void Shader::PushRawSource(const char* shaderName, const char* shaderSource, ShaderType type)
	{
		ShaderObj obj = ShaderObj(shaderName, NULL, type);
		if(!success)
			return;

		log_debug("Compiling shader %s", shaderName);
		//compile(obj);
		obj.shader = RendererAPI::CompileShader(shaderSource, type);
		shader_objs.push_back(obj);

		if(obj.shader<0)
			success = 0;
	}

	bool Shader::compile(ShaderObj obj)
	{
		if(!obj.source)
			return false;


		obj.shader = RendererAPI::CompileShader(obj.source, obj.type);
		shader_objs.push_back(obj);
		free(obj.source);

		if(obj.shader<0)
			success = 0;

		return success;
	}

	char* Shader::readShaderFile(const char* path) {
		FILE *shader_file;
		//log_debug("Loading shader %s", path);

		shader_file = fopen(path,"r");
		if(!shader_file){
			log_error("Unable to open shader %s, bailing out!", path);
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
			log_error("Failed to read shader %s, bailing out!", path);
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
		if(!success) return 0;

		program = ShaderProgram::Create();

		for( auto& shobj : shader_objs ) {
			program->AttachShader(shobj.shader);
		}

		success = program->Link();

		for( auto& shobj : shader_objs ) {
			RendererAPI::DestroyShader(shobj.shader);
		}

		return success;
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

    // FIXME : Fix uniform caching for formatted uniform names 
	int Shader::GetUniformLocation(const char* uniformName)
	{
		if(!success) return -1;
        int loc;

        if(uniform_cache_map.find(uniformName) == uniform_cache_map.end()) {
			loc = RendererAPI::GetShaderUniformLocation(program->GetID(), uniformName);
        }

        //if(loc<0) log_error("getUniformLocation() : Invalid uniform %s", uniformName);
        return loc;
	}

	int Shader::GetAttribLocation(const char* attribName)
	{
		if(!success) return -1;

		return RendererAPI::GetShaderAttribLocation(program->GetID(), attribName);
	}

}
