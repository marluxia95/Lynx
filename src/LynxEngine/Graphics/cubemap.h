#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <stdio.h>
#include <vector>
#include <memory>
#include "shader.h"
#include "vertexArray.h"
#include "texture.h"
#include "lynx_common.h"

namespace Lynx::Graphics {

	class LYNXENGINE_API Cubemap{
		public:
			Cubemap(std::shared_ptr<TextureBase> texture);
			~Cubemap();
			void Use(glm::mat4 projection, glm::mat4 view); 
			
		private:
			std::shared_ptr<TextureBase> texture;
			std::shared_ptr<Shader> shader;
			std::unique_ptr<VertexArray> VAO;
			std::unique_ptr<VertexBuffer> VBO;
	};

}

#endif