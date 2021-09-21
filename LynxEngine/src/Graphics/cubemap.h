#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <stdio.h>
#include <vector>
#include <memory>
#include "shader.h"
#include "vertexArray.h"

namespace Lynx::Graphics {

	class Cubemap {
		public:
			Cubemap(std::vector<const char*>* textures);
			~Cubemap();
			void Use(glm::mat4 projection, glm::mat4 view);
			
		private:
			unsigned int textureID;
			Shader* shader;
			VertexArray* VAO;
			GLuint VBO;
	};

}

#endif