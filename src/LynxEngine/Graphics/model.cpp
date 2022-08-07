#include "Core/application.h"
#include "Core/entity_manager.h"
#include "Core/logger.h"
#include "renderer.h"
#include "mesh.h"
#include "material.h"
#include "model.h"

namespace Lynx::Graphics {

Entity* ModelLoader::LoadModel(const char* path)
{
    auto cached_mesh = Lynx::Application::GetSingleton()->GetResourceManager()->GetResource<Graphics::Mesh>(path);
    if(cached_mesh)
	{
        Entity* ent = m_entityManager->CreateEntity();
        Renderable* rndl = new Renderable();
        rndl->SetMesh(cached_mesh);
        ent->SetRenderObj(rndl);
        return ent;
    }

    const aiScene *ai_scene = m_importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	

	if(!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
	{
		log_error("ModelLoader : Error while loading model ! %s \n", m_importer.GetErrorString());
		LYNX_ASSERT(false, "Error while loading assets");
	}

	log_debug("ModelLoader : Number of children nodes %d", ai_scene->mRootNode->mNumChildren);

    // FIXME : Change this shitty model loader lol 

	return LoadNode(path, ai_scene->mRootNode, ai_scene);
}

Entity* ModelLoader::LoadNode(const char* path, aiNode* node, const aiScene* ai_scene)
{
	Application* applicationInstance = Lynx::Application::GetSingleton();

	Entity* rootEnt = m_entityManager->CreateEntity();
	
	if(ai_scene->HasMeshes() != true) {log_error("ModelLoader : File has no meshes !"); return rootEnt;}

	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = ai_scene->mMeshes[node->mMeshes[i]];
		rootEnt->SetRenderObj(ParseMesh(path, ai_scene, mesh));
		log_debug("Added mesh renderer");
		break;
	}
	
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		if(!node->mChildren[i]->mNumMeshes)
			continue;
		
		rootEnt->AddChild(LoadNode(path, node->mChildren[i], ai_scene));
		log_debug("Added child entity");
	}

	log_debug("ModelLoader : Processed node");

	return rootEnt;
}

Renderable* ModelLoader::ParseMesh(const char* path, const aiScene* ai_scene, aiMesh* mesh)
{
	Application* applicationInstance = Lynx::Application::GetSingleton();
	std::vector<Graphics::Vertex>* vertices = new std::vector<Graphics::Vertex>();
	std::vector<unsigned int>* indices = new std::vector<unsigned int>();
	log_debug("ModelLoader : Starting to process mesh");
	// Process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//log_debug("Creating vertex %d", i);
		Graphics::Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		//log_debug("Vertex Pos %f %f %f", mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		//log_debug("Vertex Normal %f %f %f", mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.Normal = vector;
		if (mesh->mTextureCoords[0]) {
			glm::vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			//log_debug("Texture pos %f %f", mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.TextureCoords = texCoord;
		}
		else {
			vertex.TextureCoords = glm::vec2(0.0f);
		}

		vertices->push_back(vertex);
	}
	log_debug("ModelLoader : Vertices processed");
	// Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices->push_back(face.mIndices[j]);
		}
	}
	log_debug("ModelLoader : Indices processed");

	// Dirty af
	Renderable* rndl = new Renderable();
	rndl->SetMesh( Lynx::Application::GetSingleton()->GetResourceManager()->LoadMesh(path, vertices, indices, Graphics::MESH_3D_TEXTURED_NORMAL) );
	return rndl;
}

Material ModelLoader::ParseMaterial(aiMaterial* mat)
{
	// WIP

	return Material{};
}

}
