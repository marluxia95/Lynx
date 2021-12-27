#include "Core/application.h"
#include "model.h"

namespace Lynx {



EntityID ModelLoader::loadModel(Scene* scene, const char* path, Graphics::Shader* shader)
{
	Assimp::Importer importer;
	const aiScene *ai_scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
	Application* applicationInstance = Lynx::Application::GetInstance();
	
	if(!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode) 
	{
		log_error("Error while loading model ! %s \n", importer.GetErrorString());
		return NULL;
	}

	EntityID parentEnt = scene->CreateEntity(path);
	
	scene->AddComponent<Transform>(parentEnt, Transform{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)});

	processNode(scene, parentEnt, path, shader, ai_scene->mRootNode, ai_scene);

	return parentEnt;
}

void ModelLoader::processMesh(Scene* scene, EntityID meshEntity, const char* path, Graphics::Shader* meshShader, aiMesh* mesh)
{
	Application* applicationInstance = Lynx::Application::GetInstance();
	std::vector<Graphics::Vertex>* vertices = new std::vector<Graphics::Vertex>();
	std::vector<unsigned int>* indices = new std::vector<unsigned int>();
	log_debug("Starting to process mesh");
	// Process vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) 
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
		if(mesh->mTextureCoords[0]){
			glm::vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			//log_debug("Texture pos %f %f", mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.TextureCoords = texCoord;
		}else{
			vertex.TextureCoords = glm::vec2(0.0f);
		}
		
		vertices->push_back(vertex);
	}
	log_debug("Vertices processed");
	// Process indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++){
			indices->push_back(face.mIndices[j]);
		}
	}  
	log_debug("Indices processed");
	scene->AddComponent<MeshRenderer>(meshEntity, 
	MeshRenderer{glm::vec3(0.0f), 
	glm::vec3(0.0f), 
	glm::vec3(0.0f), 
	0.0f, 
	std::vector<Graphics::Mesh*>{GameApplication::GetGameInstance()->GetResourceManager()->LoadMesh(path, vertices, indices, Graphics::MESH_3D_TEXTURED_NORMAL)},
	meshShader
	});
}


void ModelLoader::processNode(Scene* scene, EntityID parentEntity, const char* path, Graphics::Shader* shader, aiNode* node, const aiScene* ai_scene)
{
	Application* applicationInstance = Lynx::Application::GetInstance();
	if(ai_scene->HasMeshes() != true) {log_error("File has no meshes !"); return;}
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		
		aiMesh *mesh = ai_scene->mMeshes[node->mMeshes[i]]; 
		//applicationInstance->GetComponent<Generic>(meshEntity)->isChild = true;
		processMesh(scene, parentEntity, path, shader, mesh);
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(scene, parentEntity, path, shader, node->mChildren[i], ai_scene);
	}
}

}

