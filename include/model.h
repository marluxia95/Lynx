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

#include "simpleGameEngine.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "components.h"
#include "texture.h"
#include "logger.h"
#include "shader.h"
#include "entity.h"
#include "model.h"


using namespace std;

namespace Lynx::ModelLoader {

	Entity loadModel(const char* path, Shader* shader);
	void processNode(Entity parentEntity, Shader* shader, aiNode* node, const aiScene* scene);
	void processMesh(Entity meshEntity, Shader* meshShader, aiMesh* mesh, const aiScene* scene);

}


#endif
