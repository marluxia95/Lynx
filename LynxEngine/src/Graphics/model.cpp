#include "model.h"

using namespace std;

extern Lynx::Application gApplication;
extern Lynx::ResourceManager gResourceManager;

namespace Lynx::ModelLoader {

Entity loadModel(const char* path, Shader* shader)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		log_error("Error while loading model %s \n", importer.GetErrorString());
		return NULL;
	}

	Entity parentEnt = gApplication.CreateEntity(path);
	
	gApplication.AddComponent<Transform>(parentEnt, Transform{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)});

	processNode(parentEnt, path, shader, scene->mRootNode, scene);

	return parentEnt;
}

void processMesh(Entity meshEntity, const char* path, Shader* meshShader, aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex>* vertices = new vector<Vertex>();
	vector<GLuint>* indices = new vector<GLuint>();
	log_debug("Starting to process mesh");
	// Process vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) 
	{
		log_debug("Creating vertex %d", i);
		Vertex vertex;
		glm::vec3 vector; 
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z; 
		log_debug("Vertex Pos %f %f %f", mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		log_debug("Vertex Normal %f %f %f", mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.Normal = vector; 
		if(mesh->mTextureCoords[0]){
			glm::vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			log_debug("Texture pos %f %f", mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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
	gApplication.AddComponent<MeshRenderer>(meshEntity, MeshRenderer{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, gResourceManager.LoadMesh(path, vertices, indices, MESH_3D_TEXTURED_NORMAL), meshShader});
}


void processNode(Entity parentEntity, const char* path, Shader* shader, aiNode* node, const aiScene* scene)
{
	if(scene->HasMeshes() != true) {log_error("File has no meshes !"); return;}
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
		Entity meshEntity = gApplication.CreateEntity(mesh->mName.C_Str());
		gApplication.AddComponent<Transform>(meshEntity, Transform{glm::vec3(0), glm::vec3(0), glm::vec3(1)});
		gApplication.AddComponent<Parent>(meshEntity, Parent{parentEntity});
		//gApplication.GetComponent<Generic>(meshEntity)->isChild = true;
		processMesh(meshEntity, path, shader, mesh, scene);
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(parentEntity, path, shader, node->mChildren[i], scene);
	}
}

}

