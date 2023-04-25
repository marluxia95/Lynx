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

#include "Core/entity.h"

namespace Lynx::Graphics {

    class Material;

    class LYNXENGINE_API ModelLoader {
	public:
		ModelLoader(std::shared_ptr<EntityManager> entityManager) : m_entityManager(entityManager) {}
		Entity* LoadModel(const char* path);
	protected:
		Entity* LoadNode(const char* path, aiNode* node, const aiScene* ai_scene);
		Renderable* ParseMesh(const char* path, const aiScene* ai_scene, aiMesh* mesh);
		Material ParseMaterial(aiMaterial* mat);
	private:
		std::shared_ptr<EntityManager> m_entityManager;
		Assimp::Importer m_importer;
		
	};

}

#endif
