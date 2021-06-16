#include <stdio.h>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include "simpleGameEngine.h"
#include "components.h"
#include "renderSystem.h"
#include "logger.h"

extern Lynx::Core::Game game;

using namespace glm;

namespace Lynx {
    
    
    void RenderSystem::Init()
    {
        log_debug("Creating Main Camera");
        cameraEntity = game.CreateEntity("Main camera");

        game.AddComponent(cameraEntity, Transform{
            glm::vec3(0), 
            glm::vec3(0), 
            glm::vec3(0)
        });

        game.AddComponent(cameraEntity, Camera{
            60, // Field of view
            vec2(game.WINDOW_WIDTH, game.WINDOW_HEIGHT), // Resolution
            CAMERA_PERSPECTIVE, // Camera type
            true, // Is it a main camera ?
            vec3(0.0f,0.0f,-1.0f), // Front vector
            vec3(0.0f,1.0f,0.0f)   // Up vector
        });

    }

    void RenderSystem::Update()
    {
        const auto& mCameraComponent = game.GetComponent<Camera>(cameraEntity);
        
        for (auto const& entity : entities) {
            const auto& mTransform = game.GetComponent<Transform>(entity);
            const auto& mRenderComponent = game.GetComponent<MeshRenderer>(entity);
            
            

            mRenderComponent->shader->use();

            mat4 model = mTransform->GetModel();
            
            if(mRenderComponent->shader == NULL){log_error("Invalid shader for entity %d!", entity); return;}

            mRenderComponent->shader->setMat4("projection", mCameraComponent->projection);
            mRenderComponent->shader->setMat4("view", mCameraComponent->view);
            mRenderComponent->shader->setMat4("model", model);
            mRenderComponent->shader->setVec3("color", mRenderComponent->color);

            if(mRenderComponent->mesh == nullptr){log_error("Render component not bind to a mesh"); continue;}

            // Check if mesh has a texture, if so, render it
            if(mRenderComponent->mesh->type >= MESH_3D_TEXTURED)
            {
            	if(mRenderComponent->texture != NULL){
                	mRenderComponent->texture->use();
            	}   
            }

            mRenderComponent->mesh->VAO->Bind();
            mRenderComponent->mesh->Render();
        }

    }

}
