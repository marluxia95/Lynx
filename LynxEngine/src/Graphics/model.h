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

#include "Core/application.h"
#include "Core/logger.h"

#include "Graphics/buffers.h"
#include "Graphics/texture.h"
#include "Graphics/shader.h"
#include "Graphics/model.h"

#include "Core/ECS/components.h"
#include "Core/ECS/entity.h"



using namespace std;

namespace Lynx::ModelLoader {

	Entity loadModel(const char* path, Graphics::Shader* shader);
	void processNode(Entity parentEntity, const char* path, Graphics::Shader* shader, aiNode* node, const aiScene* scene);
	void processMesh(Entity meshEntity, const char* path, Graphics::Shader* meshShader, aiMesh* mesh, const aiScene* scene);

}


#endif
