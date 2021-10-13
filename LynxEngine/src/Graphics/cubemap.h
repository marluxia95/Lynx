#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <stdio.h>
#include <vector>
#include <memory>
#include "shader.h"
#include "vertexArray.h"
#include "texture.h"

namespace Lynx::Graphics {

	class CubemapTexture : public TextureBase {
		public:
			CubemapTexture();
			void Load();
			void Load(const char* path) { loadFromFile(path); }
			void Generate();
			void Use();
			friend class Cubemap;
		private:
			int count = 0;
	};

	class Cubemap{
		public:
			Cubemap();
			~Cubemap();
			void Load(std::vector<const char*>* textures);
			void Use(glm::mat4 projection, glm::mat4 view);
			
		private:
			CubemapTexture texture;
			Shader* shader;
			VertexArray* VAO;
			GLuint VBO;
	};

}

#endif