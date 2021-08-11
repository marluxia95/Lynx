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

#include "Core/ECS/components.h"
#include "Systems/renderSystem.h"
#include "Systems/lightingSystem.h"

using namespace glm;

extern Lynx::Application gApplication;
extern Lynx::WindowManager gWindowManager;

namespace Lynx {

    Entity RenderSystem::CreatePerspectiveCamera()
    {
        Entity entity = gApplication.CreateEntity("Main camera");
        glm::vec2 resolution = glm::vec2(gApplication.GetResolutionWidth(), gApplication.GetResolutionHeight());
        
        gApplication.AddComponent(entity, Transform{
            glm::vec3(0), 
            glm::vec3(0), 
            glm::vec3(0)
        });

        gApplication.AddComponent(entity, Camera{
            resolution.x,
            resolution.y,
            CAMERA_PERSPECTIVE,
            60.0f,
            0.0f,
            1000.0f
        });

        return entity;
    }

    Entity RenderSystem::CreateOrthographicCamera()
    {
        Entity entity = gApplication.CreateEntity("Main camera");
        glm::vec2 resolution = glm::vec2(gApplication.GetResolutionWidth(), gApplication.GetResolutionHeight());

        gApplication.AddComponent(entity, Transform{
            glm::vec3(0), 
            glm::vec3(0), 
            glm::vec3(0)
        });

        // Add camera component with default values
        gApplication.AddComponent(entity, Camera{
            resolution.x,
            resolution.y,
            CAMERA_ORTHOGRAPHIC,
            NULL,
            0.0f,
            1.0f,
        });

        return entity;
    }

    Entity RenderSystem::CreateDirectionalLight(glm::vec3 direction, glm::vec3 ambientColor)
    {
        Entity directionalLight = gApplication.CreateEntity("Directional Light");


        gApplication.AddComponent(directionalLight, Transform{
            glm::vec3(0), 
            glm::vec3(0), 
            glm::vec3(0)
        });

        gApplication.AddComponent(directionalLight, DirectionalLight{
            direction,
            ambientColor,
            glm::vec3(1.0f),
            glm::vec3(0.8f),
            0.2f
        });

        return directionalLight;
    }

    Entity RenderSystem::GetMainCamera() 
    {
        return cameraEntity;
    }

    Entity RenderSystem::GetDirectionalLight()
    {
        return directionalLight;
    }
    
    void RenderSystem::Init()
    {
        lightingSystem = gApplication.GetSystem<LightingSystem>();
    }

    void RenderSystem::SetMainCamera(Entity cameraEnt)
    {
        cameraEntity = cameraEnt;
        log_debug("Successfully set main camera to %d\n", cameraEnt);
    }

    void RenderSystem::SetDirectionalLight(Entity dirLight)
    {
        directionalLight = dirLight;
        log_debug("Successfully set directional light to %d\n", dirLight);
    }

    void RenderSystem::SetRenderMode(RenderMode mode)
    {
        renderMode = mode;
        log_debug("Successfully set rendering type to %d\n", ( mode == RENDER_2D ? "RENDER_2D" : "RENDER_3D"));
    }

    void RenderSystem::Update()
    {   
        render3D();
    }

    void RenderSystem::render3D()
    {
        const auto& mCameraComponent = gApplication.GetComponent<Camera>(cameraEntity);
        const auto& mCameraTransform = gApplication.GetComponent<Transform>(cameraEntity);
        const auto& mDirLightComponent = gApplication.GetComponent<DirectionalLight>(directionalLight);

        for (auto const& entity : entities) {
            const auto& mTransform = gApplication.GetComponent<Transform>(entity);
            const auto& mRenderComponent = gApplication.GetComponent<MeshRenderer>(entity);

            mRenderComponent->shader->use();
            mat4 model = mTransform->GetModel();
            
            if(mRenderComponent->shader == NULL){log_error("Invalid shader for entity %d!", entity); return;}

            mRenderComponent->shader->setMat4("projection", mCameraComponent->projection);
            mRenderComponent->shader->setMat4("view", mCameraComponent->view);
            mRenderComponent->shader->setMat4("model", model);
            mRenderComponent->shader->setVec3("color", mRenderComponent->ambient);
            mRenderComponent->shader->setVec3("viewPos", mCameraTransform->position);
			
			if(mRenderComponent->lighting | lightingSystem->entities.size()){
				// Set lighting shader values
				int i = 0;
				for (auto lightEnt : lightingSystem->entities){
					auto lightComponent = gApplication.GetComponent<PointLight>(lightEnt);
                    auto transform = gApplication.GetComponent<Transform>(lightEnt);
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
            
            // Check if mesh has a texture, if so, use it
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

    void RenderSystem::render2D()
    {
        const auto& mCameraComponent = gApplication.GetComponent<Camera>(cameraEntity);
        const auto& mCameraTransform = gApplication.GetComponent<Transform>(cameraEntity);

        for (auto const& entity : entities) {
            const auto& mTransform = gApplication.GetComponent<Transform>(entity);
            const auto& mRenderComponent = gApplication.GetComponent<MeshRenderer>(entity);

            if(mRenderComponent->mesh->type > MESH_2D_SPRITE)
                continue;

            // Check if sprite has a texture, if so, use it
            if(mRenderComponent->mesh->type >= MESH_3D_TEXTURED)
            {
                    
                if(mRenderComponent->texture != nullptr){    
                    mRenderComponent->texture->use();
                }   
            }

            mRenderComponent->mesh->Render();

        }

    }

}
