#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <memory>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <stdarg.h>
#include <string.h>
#include "Core/resource.h"
#include "graphics_api.h"

//#define SHADER_DEBUG
#define MAX_ERR_BUFSIZE 2048

namespace Lynx {  

    // Abstract base class for shader program
	class ShaderProgram {
	public:
		ShaderProgram() = default;
		~ShaderProgram() = default;
		unsigned int GetID() { return id; };
		char* GetErrorLog() { return error_log; };
		virtual void Use() = 0;
		virtual void AttachShader(unsigned int shaderID) = 0;
		virtual bool Link() = 0;

		static std::unique_ptr<ShaderProgram> Create();
	protected:
		unsigned int id;
		char error_log[MAX_ERR_BUFSIZE];
	};

	class LYNXENGINE_API Shader : public Resource {
	private:
		class ShaderObj {
		public:
			ShaderObj(const char* path, char* source, ShaderType type) : path(path), source(source), type(type) { }
			const char* path;
			char* source;
			ShaderType type;
			unsigned int shader;
		};  


		bool compile(ShaderObj obj);
		void loadShaderFromFile(const char* file);
		char* readShaderFile(const char* file);

	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		Shader();

		void PushSource(const std::string& shaderPath, ShaderType type);
		void PushRawSource(const char* shaderName, const char* shaderSource, ShaderType type);

		void Use();
		int  GetUniformLocation(const char* name);
		int  GetAttribLocation(const char* name);
		bool Link();

		template<typename T>
			void SetUniform(const char* name, T value)
		{
			if(!success) return;

			auto location = GetUniformLocation(name);
			API_CheckErrors();
			RendererAPI::SetShaderUniform(location, value);
		}

		template<typename T>
			void SetUniformf(const char* format, T value, ...)
		{
			if(!success) return;

			va_list ap;

			va_start(ap, format);

			int l = vsnprintf(uniformBuf, sizeof(uniformBuf), format, ap);
			va_end(ap);

			auto location = GetUniformLocation(uniformBuf);
			API_CheckErrors();
			RendererAPI::SetShaderUniform(location, value);
		}

		bool Reload();
		char* GetError() { return errorlog; }

		bool success = true;
	private:
		size_t shaderSize;
		char uniformBuf[64];
		char* errorlog;

		std::unique_ptr<ShaderProgram> program;

		std::map<const char*, int> uniform_cache_map;
		std::map<const char*, int> attrib_cache_map;
		std::vector<ShaderObj> shader_objs;
	};

}

#endif
