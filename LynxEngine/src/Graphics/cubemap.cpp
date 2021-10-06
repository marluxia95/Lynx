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

	TextureData* CubemapTexture::Load(const char* path) 
	{
		TextureData* tdata = loadTexture(path);
	}

	void CubemapTexture::Generate(TextureData* data) 
	{
		log_debug("Generating cubemap texture");
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		switch ( IRendererAPI::GetAPI() ) {
			case API_OPENGL:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + count, 0, GL_RGB, data->width, data->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data->data);
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
		}
		count++;
	}

	void CubemapTexture::Use()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	}

	Cubemap::Cubemap()
	{
	}

	void Cubemap::Load(std::vector<const char*>* textures)
	{
		Lynx::GameApplication* applicationInstance = GameApplication::GetGameInstance();
        log_debug("Creating cubemap ...");
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
	    applicationInstance->GetResourceManager()->LoadCubemapTexture(textures);
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
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

}
