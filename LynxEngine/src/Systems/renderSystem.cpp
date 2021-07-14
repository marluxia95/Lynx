#include <stdio.h>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Core/simpleGameEngine.h"
#include "Core/windowManager.h"
#include "Core/logger.h"

#include "ECS/components.h"
#include "Systems/renderSystem.h"


using namespace glm;

extern Lynx::Game game;
extern Lynx::WindowManager gWindowManager;

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
            vec2(gWindowManager.window_width, gWindowManager.window_height), // Resolution
            CAMERA_PERSPECTIVE, // Camera type
            true, // Is it a main camera ?
            vec3(0.0f,1.0f,0.0f)   // Up vector
        });

        directionalLight = game.CreateEntity("Directional Light");

        game.AddComponent(directionalLight, Transform{
            glm::vec3(0), 
            glm::vec3(0), 
            glm::vec3(0)
        });

        game.AddComponent(directionalLight, DirectionalLight{
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f),
            glm::vec3(1.0f),
            glm::vec3(0.8f),
            0.2f
        });

    }

    void RenderSystem::Update()
    {
        const auto& mCameraComponent = game.GetComponent<Camera>(cameraEntity);
        const auto& mCameraTransform = game.GetComponent<Transform>(cameraEntity);
        const auto& mDirLightComponent = game.GetComponent<DirectionalLight>(directionalLight);

        for (auto const& entity : entities) {
            const auto& mTransform = game.GetComponent<Transform>(entity);
            const auto& mRenderComponent = game.GetComponent<MeshRenderer>(entity);

            mRenderComponent->shader->use();
            mat4 model = mTransform->GetModel();
            
            if(mRenderComponent->shader == NULL){log_error("Invalid shader for entity %d!", entity); return;}

            mRenderComponent->shader->setMat4("projection", mCameraComponent->projection);
            mRenderComponent->shader->setMat4("view", mCameraComponent->view);
            mRenderComponent->shader->setMat4("model", model);
            mRenderComponent->shader->setVec3("color", mRenderComponent->ambient);
            mRenderComponent->shader->setVec3("viewPos", mCameraTransform->position);
			
			if(mRenderComponent->lighting | game.lightingSystem->entities.size()){
				// Set lighting shader values
				int i = 0;
				for (auto lightEnt : game.lightingSystem->entities){
					auto lightComponent = game.GetComponent<PointLight>(lightEnt);
                    auto transform = game.GetComponent<Transform>(lightEnt);
					char buffer[64];
                    
					sprintf(buffer, "pointLights[%d].position", i);
					mRenderComponent->shader->setVec3(buffer , transform->position);
					
					sprintf(buffer, "pointLights[%d].constant", i);
					mRenderComponent->shader->setFloat(buffer, lightComponent->constant);
					
					sprintf(buffer, "pointLights[%d].linear", i);
					mRenderComponent->shader->setFloat(buffer, lightComponent->linear);
					
					sprintf(buffer, "pointLights[%d].quadratic", i);
					mRenderComponent->shader->setFloat(buffer, lightComponent->quadratic);
					
					sprintf(buffer, "pointLights[%d].ambient", i);
					mRenderComponent->shader->setVec3(buffer, lightComponent->ambient);

                    mRenderComponent->shader->setVec3("material.ambient", mRenderComponent->ambient);
					mRenderComponent->shader->setVec3("material.diffuse", mRenderComponent->diffuse);
                    mRenderComponent->shader->setVec3("material.specular", mRenderComponent->specular);
                    mRenderComponent->shader->setFloat("material.shininess", mRenderComponent->shininess);

                    mRenderComponent->shader->setVec3("directionalLight.direction", mDirLightComponent->direction);
                    mRenderComponent->shader->setVec3("directionalLight.ambient", mDirLightComponent->ambient);
                    mRenderComponent->shader->setVec3("directionalLight.diffuse", mDirLightComponent->diffuse);
                    mRenderComponent->shader->setVec3("directionalLight.specular", mDirLightComponent->specular);
                    
                    mRenderComponent->shader->setFloat("directionalLight.intensity", mDirLightComponent->intensity);
                    

					i++;
				}
			}


            if(mRenderComponent->mesh == nullptr){log_error("Render component not bind to a mesh"); continue;}
            
            // Check if mesh has a texture, if so, render it
            if(mRenderComponent->mesh->type >= MESH_3D_TEXTURED)
            {
                
            	if(mRenderComponent->texture != nullptr){
                    
                	mRenderComponent->texture->use();
            	}   
            }

            mRenderComponent->mesh->VAO->Bind();
            mRenderComponent->mesh->Render();
        }

    }

}
