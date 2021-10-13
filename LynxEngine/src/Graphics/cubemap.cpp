#include <glm/glm.hpp>
#include "Core/logger.h"
#include "cubemap.h"
#include "texture.h"
#include "Core/application.h"
#include "Graphics/rendererAPI.h"

namespace Lynx::Graphics {

	float cubemapVertices[] {
		-1.0f,  1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,

	    -1.0f, -1.0f,  1.0f,
	    -1.0f, -1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,

	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,

	    -1.0f, -1.0f,  1.0f,
	    -1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,

	    -1.0f,  1.0f, -1.0f,
	     1.0f,  1.0f, -1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	    -1.0f,  1.0f,  1.0f,
	    -1.0f,  1.0f, -1.0f,

	    -1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f
	};

	CubemapTexture::CubemapTexture() : TextureBase(TEXTURE_CUBEMAP) 
	{
		glGenTextures(1, &texture);
	    
	}

	void CubemapTexture::Load()
	{
		if(data->GetPath() == NULL)
			log_warn("CubemapTexture::Load() : Texture data must include a path");
		else
			loadFromFile(data->GetPath());
	}

	void CubemapTexture::Generate() 
	{
		if(data == nullptr | data == nullptr | data->GetPath() == NULL) { log_error("CubemapTexture::Generate() : Invalid texture data"); return; }
		
		log_debug("Cubemap texture generate : %s", data->GetPath());
		
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + count, 0, GL_RGB, data->GetWidth(), data->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, data->GetData());
				unsigned char* rawdata = data->GetData();
				log_debug("Texture data : Path=%s W=%d H=%d Data length=%d", data->GetPath(), data->GetWidth(), data->GetHeight(), sizeof(rawdata) / sizeof(*rawdata));
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
		}
		API_CheckErrors();
		log_debug("After cubemap texture generate");
		count++;
		data->Free();
		if(count==6) {
			id = TextureBase::PushTextureID();
			log_debug("New ID for cubemap texture : %d, texture %d", id, texture);
		}
	}

	void CubemapTexture::Use()
	{
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL:
				glActiveTexture(GL_TEXTURE0 + id);
				glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
				break;
		}
	}

	Cubemap::Cubemap()
	{
	}

	void Cubemap::Load(std::vector<const char*>* textures)
	{
		Lynx::GameApplication* applicationInstance = GameApplication::GetGameInstance();
        log_debug("Creating cubemap ...");

		glGenTextures(1, &texture.texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.texture);

        VAO = VertexArray::Create();
		glGenBuffers(1, &VBO);
		VAO->Bind();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);  
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		log_debug("Loading cubemap shader...");
	    shader = applicationInstance->GetResourceManager()->LoadShader("res/shaders/standard/cubemap.shader");

        log_debug("Starting to load cubemap textures...");
	    texture = applicationInstance->GetResourceManager()->LoadCubemapTexture(textures);
        log_debug("Cubemap created successfully");
	}

	Cubemap::~Cubemap()
	{
		delete VAO, shader;
	}

	void Cubemap::Use(glm::mat4 projection, glm::mat4 view)
	{
		glDepthFunc(GL_LEQUAL);
		shader->Use();
		view = glm::mat4(glm::mat3(view));
		shader->SetUniform("projection", projection);
		shader->SetUniform("view", view);
		VAO->Bind();

		texture.Use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

}
