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
#include <list>
#include <GL/glew.h> 
#include "shader.h"
#include "rendererAPI.h"
#include "Platform/OpenGL/GLShader.h"
#include "Core/logger.h"
#include "Core/assert.h"

namespace Lynx::Graphics {

	ShaderProgram* ShaderProgram::Create()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL: return new Graphics::OpenGL::GLShaderProgram();
			default : return nullptr;
		}
	}

	Shader::Shader(const char* path) {
		loadShaderFromFile(path);
	}

	bool Shader::compile(const char* source)
	{
		std::list<unsigned int> shader_ids;
		std::string shaderbuf;
		std::string source_str(source);
		std::istringstream stream(source_str);
		std::string line;
		unsigned int section = 0;
		int isheader;
		char shadertype_header[64];
		size_t size;
		
		API_CheckErrors();

		// Create shader program
		log_debug("Creating shader program", shader_path);
		program = ShaderProgram::Create();

		
		while(std::getline(stream, line)) {	
			isheader = sscanf(line.c_str(), "#shader %s", &shadertype_header);
			//size_t shaderbuf_size = strlen(shaderbuf);
			size_t line_size = line.size();
#ifdef SHADER_DEBUG
			log_debug("Line %d : %s", isheader, line.c_str());
#endif

			if(section&&!isheader) {
				shaderbuf = shaderbuf + line + '\n';
				//log_debug("ShaderBuffer : %s", shaderbuf.c_str());
			}

			if(isheader > 0) { 
				if(section) {
#ifdef SHADER_DEBUG
					log_debug("Section end %d", section);
					log_debug("Total section : %s", shaderbuf.c_str());
#endif
					// Compile buffer
					unsigned int shid = RendererAPI::CompileShader(shaderbuf.c_str(), (ShaderType)section);
					API_CheckErrors();
					program->AttachShader(shid);
					shader_ids.push_back(shid); // Store the shader's id into the list to remove it after program is linked
				}

				section = StrToShaderType(shadertype_header);
				//log_debug("New section %d", section);
				shaderbuf.clear();
			}
		}

		// Last line
		if(section) {
#ifdef SHADER_DEBUG
			log_debug("Total section : %s", shaderbuf.c_str());
			log_debug("File end");
#endif
			// Compile buffer
			shaderbuf += '\0';
			unsigned int shid = RendererAPI::CompileShader(shaderbuf.c_str(), (ShaderType)section);
			API_CheckErrors();
			program->AttachShader(shid);
			shader_ids.push_back(shid); // Store the shader's id into the list to remove it after program is linked
		}

		log_debug("Linking shader program");
		program->Link();

		log_debug("Destroying shaders");
		auto l_front = shader_ids.begin();
		// Destroy all shaders, since program is linked now
		for( int id = 0; id < shader_ids.size(); id++) {
			std::advance(l_front, 1);
			RendererAPI::DestroyShader(*l_front);
		}

		API_CheckErrors();
	}


	void Shader::loadShaderFromFile(const char* source_file) 
	{
		FILE *shader_file;
		log_debug("Loading shader %s", source_file);
		
		shader_path = source_file;

		shader_file = fopen(source_file,"r");
		if(!shader_file){log_error("Unable to open shader %s", source_file); return;}

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
			log_error("Failed to read shader %s", source_file);
			free(shader_source);
			success = false;
			return;
		}

		log_debug("Compiling shader %s", shader_path);
		compile(shader_source);

		free(shader_source);

	}

	/**
	 * @brief Reloads the shader
	 * 
	 * @return bool True if shader was compiled successfully, false if there was an error while compiling the shader
	 */
	bool Shader::Reload(){
		LYNX_ASSERT(shader_path && program, "Invalid shader");

		loadShaderFromFile(shader_path);

		return success;
	}

	/**
	 * @brief Destroys the shader's program
	 * 
	 */
	void Shader::Destroy(){
		if(program && success)
			delete program;
	}

	/**
	 * @brief Uses the shader's program
	 * 
	 */
	void Shader::Use(){
		if(program && success)
			program->Use();
			API_CheckErrors();
	}

	int Shader::getUniformLocation(const char* uniformName)
	{
		if(uniform_cache_map.find(uniformName) == uniform_cache_map.end()) {
			int loc = RendererAPI::GetShaderUniformLocation(program->GetID(), uniformName);
			API_CheckErrors();
			uniform_cache_map.insert({uniformName, loc});
			return loc;
		}else{
			return uniform_cache_map[uniformName];
		}
	}


}