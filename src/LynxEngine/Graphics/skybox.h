#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdio.h>
#include <vector>
#include <memory>
#include "shader.h"
#include "texture.h"

namespace Lynx::Graphics {

	class LYNXENGINE_API Skybox {
		public:
			Skybox(std::shared_ptr<TextureBase> texture);

			std::shared_ptr<TextureBase> GetTexture();
			std::shared_ptr<Shader> GetShader();
			std::shared_ptr<Mesh> GetMesh();

		private:
			std::shared_ptr<TextureBase> cube_texture;
			std::shared_ptr<Shader> cube_shader;
			std::shared_ptr<Mesh> cube_mesh;
	};

}

#endif