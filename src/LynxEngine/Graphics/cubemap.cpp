#include <glm/glm.hpp>
#include "Core/logger.h"
#include "cubemap.h"
#include "texture.h"
#include "Core/application.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {

	// WIP

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

	Cubemap::Cubemap(std::shared_ptr<TextureBase> texture) : texture(texture)
	{
		Lynx::GameApplication* applicationInstance = GameApplication::GetGameInstance();
        log_debug("Creating cubemap ...");

		log_debug("Loading cubemap shader...");
	    shader = applicationInstance->GetResourceManager()->LoadShader("res/shaders/standard/cubemap.vert", "res/shaders/standard/cubemap.frag");

		//glGenTextures(1, &texture->texture);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, texture->texture);

        VAO = VertexArray::Create();
		VBO = VertexBuffer::Create();
		VAO->Bind();
		VBO->SetData(&cubemapVertices, sizeof(cubemapVertices));
		glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//VBO->Configure(MESH_3D);
		/*glBindBuffer(GL_ARRAY_BUFFER, VBO);  
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
		*/
	}

	Cubemap::~Cubemap()
	{

	}

	void Cubemap::Use(glm::mat4 projection, glm::mat4 view)
	{
		glDepthFunc(GL_LEQUAL);
		shader->Use();
		view = glm::mat4(glm::mat3(view));
		shader->SetUniform("projection", projection);
		shader->SetUniform("view", view);
		VAO->Bind();

		texture->Use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

}
