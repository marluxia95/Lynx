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
#include "Core/scene.h"

#include "mesh.h"

namespace Lynx::Graphics {

	class LYNXENGINE_API ModelLoader {
	public:
		ModelLoader() {}
		EntityHandlePtr LoadModel(const char* path);
		EntityHandlePtr LoadNode(const char* path, aiNode* node, const aiScene* ai_scene);
		RenderHndlPtr ProcessMesh(const char* path, aiMesh* mesh);
	private:
		Assimp::Importer m_importer;
		
	};

}

#endif
