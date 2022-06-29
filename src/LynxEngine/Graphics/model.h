#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <string.h>
#include <vector>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Core/logger.h"

#include "buffers.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"

#include "Core/scene.h"

#include "lynx_common.h"

namespace Lynx {

	class LYNXENGINE_API ModelLoader {
		private:
			Scene* scene;

		public:
			ModelLoader(Scene* scene) : scene(scene) {}
			// TODO
			//std::shared_ptr<Graphics::Mesh> ProcessMesh( const char* path, aiMesh* mesh);
		
	};

}

#endif
