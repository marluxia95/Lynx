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

	/*
	class LYNXENGINE_API CubemapTexture : public TextureBase {
		public:
			CubemapTexture();
			void Load();
			void Load(const char* path) { LoadFromFile(path); }
			void Generate();
			void Use();
			friend class Cubemap;
		private:
			int count = 0;
	};*/

	class LYNXENGINE_API Cubemap{
		public:
			Cubemap();
			~Cubemap();
			void Load(std::vector<const char*>* textures);
			void Use(glm::mat4 projection, glm::mat4 view);
			
		private:
			std::shared_ptr<TextureBase> texture;
			std::shared_ptr<Shader> shader;
			VertexArray* VAO;
			GLuint VBO;
	};

}

#endif