#include <glm/glm.hpp>
#include "Core/logger.h"
#include "Core/application.h"
#include "cubemap.h"
#include "texture.h"
#include "Graphics/rendererAPI.h"

extern Lynx::Application gApplication;

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

	Cubemap::Cubemap(std::vector<const char*>* textures)
	{
        log_debug("Creating cubemap ...");
        VAO = VertexArray::Create();
		glGenBuffers(1, &VBO);
		VAO->Bind();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);  
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		
	    glGenTextures(1, &textureID);
	    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		log_debug("Loading cubemap shader...");
	    shader = gApplication.GetResourceManager()->LoadShader("res/shaders/standard/cubemap.shader");

	    int w,h,ch;
        log_debug("Starting to load cubemap textures...");
	    for(unsigned int x = 0; x < textures->size(); x++){
            log_debug("Loading texture %s", textures->at(x));
	    	TextureData* tdata = loadTexture(textures->at(x));
	    	if(tdata->data){
				switch ( IRendererAPI::GetAPI() ) {
					case API_OPENGL:
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + x, 0, GL_RGB, tdata->width, tdata->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata->data);
						glGenerateMipmap(GL_TEXTURE_2D);
						break;
				}
	    		
            }else{
	    		log_error("Unable to load cubemap texture %s", textures->at(x));
	    	}
	    	textureData_free(tdata);
	    }
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

}
