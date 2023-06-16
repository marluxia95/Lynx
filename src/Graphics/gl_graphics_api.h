#ifndef GLRENDERERAPI_H
#define GLRENDERERAPI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <GL/glew.h>
#include "Core/logger.h"
#include "shader.h"
#include "buffers.h"
#include "vertex_array.h"
#include "graphics_api.h"

//#define API_DEBUG

GLenum checkerror_(const char* f, int l);
#ifdef __linux__
#define glCheckError() checkerror_(__PRETTY_FUNCTION__, __LINE__)
#elif _WIN32
#define glCheckError() checkerror_(__FUNCDNAME__, __LINE__)
#endif

namespace Lynx {

	class IRendererAPI;
	enum BufferType : unsigned int;

		
	class GLShaderProgram : public Lynx::ShaderProgram {
	public:
		GLShaderProgram();
		~GLShaderProgram();
		void Use();
		void AttachShader(unsigned int shaderID);
		bool Link();
	};

	class GLVertexBuffer : public Lynx::VertexBuffer {
	public:
		GLVertexBuffer(const void* data, unsigned int size);
		GLVertexBuffer(std::vector<Vertex>* vertices);
		GLVertexBuffer(std::vector<Vertex>* vertices, MeshType type);
		GLVertexBuffer(unsigned int size);
		GLVertexBuffer();
		~GLVertexBuffer();
		void SetData(const void* data, unsigned int size);
		void AddData(const void* data, unsigned int size);
		void DynamicDraw(bool dynamic_draw);
		void Bind();
		void Unbind();
		void Configure(MeshType type);
		unsigned int VBO_ID;
	private:
		bool dynamic; // GL_DYNAMIC_DRAW
	};

	class GLElementBuffer : public Lynx::ElementBuffer {
	public:
		GLElementBuffer(const void* indices, unsigned int indexNumber);
		GLElementBuffer(std::vector<unsigned int>* indices);
		GLElementBuffer(unsigned int size);
		GLElementBuffer();
		~GLElementBuffer();
		void SetData(const void* data, unsigned int size);
		void AddData(const void* data, unsigned int size);
		void DynamicDraw(bool dynamic_draw);
		void Bind();
		void Unbind();
		unsigned int ID;
		
	private:
		bool dynamic;
	};	
	
	class GLVertexArray : public Lynx::VertexArray {
	public:
		GLVertexArray();
		~GLVertexArray();
		void Bind();
		void Unbind();
		unsigned int VAO_ID;
	};
	
	class GLRendererAPI : public Lynx::IRendererAPI {
	public:
		void Init();
		
		void SetViewport(uint32_t width, uint32_t height);
		void Clear(glm::vec4 color);
		
		void LoadTexture(TextureType type, unsigned char* data, int width, int height, bool useDefaults=true, int format=0x1908); // GL_RGBA
		unsigned int GenTexture();
		void SetTextureParameter(unsigned int param, unsigned int value);
		void UseTexture(unsigned int id);
		void BindTexture(unsigned int tex);
		void BindTexture(TextureType type, unsigned int tex);
		
		void CheckErrors(const char* f, int l);
		
		unsigned int CompileShader(const char* shadersource, ShaderType type);
		int GetShaderUniformLocation(unsigned int programID, const char* uniformName);
		int GetShaderAttribLocation(unsigned int programID, const char* attribName);
		void SetShaderUniformBool(int location, bool value);
		void SetShaderUniformInt(int location, int value);
		void SetShaderUniformFloat(int location, float value);
		void SetShaderUniformVec2(int location, glm::vec2 value);
		void SetShaderUniformVec3(int location, glm::vec3 value);
		void SetShaderUniformVec4(int location, glm::vec4 value);
		void SetShaderUniformMat2(int location, glm::mat2 value);
		void SetShaderUniformMat3(int location, glm::mat3 value);
		void SetShaderUniformMat4(int location, glm::mat4 value);
		void DestroyShader(unsigned int shaderID);
		
		void DrawIndexed(int n);
		void DrawArrays(int n);
	};
}

#endif
