/**
 * @file graphics_api.cpp
 *
 * @author marlxx
 * @brief A renderer API wrapper that lets the user interact with different renderer API's
 * @version 0.0.3
 * @date 2021-09-03
 *
 * @copyright Copyright (c) 2021. See LICENSE for more information
 *
 */

#include <map>
#include <string.h>
#include "graphics_api.h"
#include "gl_graphics_api.h"


namespace Lynx {

	/*
	 *	Used for conversion
	 */
	const char* API_str[] = {
		"OpenGL"
	};

	const char* shadertype_str[] = {
		"SHADER_NULL",
		"SHADER_VERTEX",
		"SHADER_FRAGMENT",
		"SHADER_GEOMETRY",
		"SHADER_COMPUTE"
	};

	// String comparator
	struct cmp_str
	{
		bool operator()(char const *a, char const *b) const
		{
			return strcmp(a, b) < 0;
		}
	};

	// Map that contains all the correspondent strings to each shader type. Used for conversion
	std::map<const char*, ShaderType, cmp_str> str_to_shadertype_map {
		{"SHADER_NULL",		SHADER_VERTEX},
		{"SHADER_VERTEX",	SHADER_VERTEX},
		{"SHADER_FRAGMENT", SHADER_FRAGMENT},
		{"SHADER_GEOMETRY", SHADER_GEOMETRY},
		{"SHADER_COMPUTE",	SHADER_COMPUTE}
	};

	API IRendererAPI::m_API = API_OPENGL;
	std::unique_ptr<IRendererAPI> RendererAPI::m_rendererAPI = nullptr;

	std::unique_ptr<IRendererAPI> IRendererAPI::Create()
	{
		switch(m_API) {
			case API_OPENGL :
				return std::make_unique<GLRendererAPI>();
		}
		return NULL;
	}

	/**
		Returns a ShaderType value given its name
		@param str Shadertype name
		@return The corresponding ShaderType value
	*/
	ShaderType StrToShaderType(const char* str)
	{
		if(str_to_shadertype_map.find(str) == str_to_shadertype_map.end()) { log_warn("GetShaderType() : Unknown shadertype '%s'", str); return SHADER_VERTEX; }
		return str_to_shadertype_map[str];
	}

	/**
		Returns the name of a ShaderType
		@param type A ShaderType object
		@return The corresponding ShaderType string
	*/
	const char* ShaderTypeToStr(ShaderType type)
	{
		if(type > SHADER_COMPUTE) { log_warn("GetShaderType() : Unknown shadertype %d", (int)type); return nullptr; }
		return shadertype_str[(int)type];
	}

	/**
		Returns the currently used renderer API
		@return The used renderer API
	*/
	const char* IRendererAPI::GetAPIStr()
	{
		return API_str[(unsigned int)m_API];
	}

	/**
		Sets the viewport
		@param width Width of the viewport
		@param height Height of the viewport
	*/
	void RendererAPI::SetViewport(uint32_t width, uint32_t height)
	{
		m_rendererAPI->SetViewport(width, height);
	}

	/**
		Clears the display
		@param color Color to replace
	*/
	void RendererAPI::Clear(glm::vec4 color)
	{
		m_rendererAPI->Clear(color);
	}

	/**
		Loads a texture
		@param data			Texture's data
		@param width		Texture's width
		@param height		Texture's height
		@param useDefaults	Set to 'true' to use default texture values
	*/
	void RendererAPI::LoadTexture(TextureType type, unsigned char* data, int width, int height, bool useDefaults)
	{
		m_rendererAPI->LoadTexture(type, data, width, height);
	}

	/**
		Generates a texture
		@return A texture id
	*/
	unsigned int RendererAPI::GenTexture()
	{
		return m_rendererAPI->GenTexture();
	}

	/**
		Sets a texture parameter
		@param param Parameter to set
		@param value Value to set
	*/
	void RendererAPI::SetTextureParameter(unsigned int param, unsigned int value)
	{
		m_rendererAPI->SetTextureParameter(param, value);
	}

	/**
		Uses a texture
		@param id Texture's ID
	*/
	void RendererAPI::UseTexture(unsigned int id)
	{
		m_rendererAPI->UseTexture(id);
	}

	/**
		Binds a texture
		@param tex Texture's ID
	*/
	void RendererAPI::BindTexture(unsigned int tex)
	{
		m_rendererAPI->BindTexture(tex);
	}

	/**
		Binds a texture
		@param tex Texture's ID
	*/
	void RendererAPI::BindTexture(TextureType type, unsigned int tex)
	{
		m_rendererAPI->BindTexture(type, tex);
	}

	/**
		Draws in indexed mode
		@param n Number of indices to draw
	*/
	void RendererAPI::DrawIndexed(int n)
	{
		m_rendererAPI->DrawIndexed(n);
	}

	/**
		Draws in arrays mode
		@param n Number of vertices to draw
	*/
	void RendererAPI::DrawArrays(int n)
	{
		m_rendererAPI->DrawArrays(n);
	}

	/**
		Checks API errors
	*/
	void RendererAPI::CheckErrors(const char* f, int l)
	{
		m_rendererAPI->CheckErrors(f, l);
	}

	/**
		Gets latest API errors
		@return A pointer to the latest error
	*/
	char* RendererAPI::GetError()
	{
		return m_rendererAPI->GetError();
	}

	/**
		Compiles a shader
		@param shaderSource The shader's source code
		@param type The shader's type
		@return A shader ID
	*/
	unsigned int RendererAPI::CompileShader(const char* shadersource, ShaderType type)
	{
		return m_rendererAPI->CompileShader(shadersource, type);
	}

	/**
	 * @brief Gets a shader uniform's location
	 *
	 * @param programID	  Shader program's ID
	 * @param uniformName Uniform's name
	 * @return The uniform's location
	 */
	int RendererAPI::GetShaderUniformLocation(unsigned int programID, const char* uniformName)
	{
		return m_rendererAPI->GetShaderUniformLocation(programID, uniformName);
	}

	int RendererAPI::GetShaderAttribLocation(unsigned int programID, const char* attribName)
	{
		return m_rendererAPI->GetShaderAttribLocation(programID, attribName);
	}
	
	template<> void RendererAPI::SetShaderUniform<bool>(int location, bool value)
	{
		return m_rendererAPI->SetShaderUniformBool(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<int>(int location, int value)
	{
		return m_rendererAPI->SetShaderUniformInt(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<float>(int location, float value)
	{
		return m_rendererAPI->SetShaderUniformFloat(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<glm::vec2>(int location, glm::vec2 value)
	{
		return m_rendererAPI->SetShaderUniformVec2(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<glm::vec3>(int location, glm::vec3 value)
	{
		return m_rendererAPI->SetShaderUniformVec3(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<glm::vec4>(int location, glm::vec4 value)
	{
		return m_rendererAPI->SetShaderUniformVec4(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<glm::mat2>(int location, glm::mat2 value)
	{
		return m_rendererAPI->SetShaderUniformMat2(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<glm::mat3>(int location, glm::mat3 value)
	{
		return m_rendererAPI->SetShaderUniformMat3(location, value);
	}

	template<> void RendererAPI::SetShaderUniform<glm::mat4>(int location, glm::mat4 value)
	{
		return m_rendererAPI->SetShaderUniformMat4(location, value);
	}


	/**
		Destroys a shader
		@param shaderID The shader's ID
	*/
	void RendererAPI::DestroyShader(unsigned int shaderID)
	{
		m_rendererAPI->DestroyShader(shaderID);
	}


}

void check_api_errors(const char* f, int l)
{
	Lynx::RendererAPI::CheckErrors(f,l);
}
