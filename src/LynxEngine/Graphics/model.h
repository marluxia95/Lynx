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

#include "material.h"
#include "mesh.h"


namespace Lynx::Graphics {

	class LYNXENGINE_API ModelLoader {
	public:
		ModelLoader(Scene* scene) : m_scene(scene) {}
		Entity* LoadModel(const char* path);
	protected:
		Entity* LoadNode(const char* path, aiNode* node, const aiScene* ai_scene);
		Renderable* ParseMesh(const char* path, const aiScene* ai_scene, aiMesh* mesh);
		Material ParseMaterial(aiMaterial* mat);
	private:
		Scene* m_scene;
		Assimp::Importer m_importer;
		
	};

}

#endif
