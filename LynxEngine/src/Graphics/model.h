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

#include "Graphics/buffers.h"
#include "Graphics/texture.h"
#include "Graphics/shader.h"
#include "Graphics/model.h"

#include "Core/scene.h"
#include "Core/ECS/components.h"
#include "Core/ECS/common.h"

namespace Lynx {

class ModelLoader {
	public:
		static EntityID loadModel(Scene* scene, const char* path, Graphics::Shader* shader);
		static void processNode(Scene* scene, EntityID parentEntity, const char* path, Graphics::Shader* shader, aiNode* node, const aiScene* ai_scene);
		static void processMesh(Scene* scene, EntityID meshEntity, const char* path, Graphics::Shader* meshShader, aiMesh* mesh);
	
};

}

#endif
