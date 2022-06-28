#include <stdio.h>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Core/application.h"
#include "Core/windowManager.h"
#include "Core/logger.h"

#include "Core/application.h"
#include "Core/ECS/components.h"

#include "renderSystem.h"
#include "cameraSystem.h"
#include "lightingSystem.h"

using namespace glm;

namespace Lynx {
    
    void RenderSystem::Init()
    {
        Lynx::GameApplication* applicationInstance = Lynx::GameApplication::GetGameInstance();
        log_debug("Creating Main Camera");
        cameraEntity = scene->CreateEntity("Main camera");
        const auto& mCameraSystem = applicationInstance->GetSystem<CameraSystem>();
        
        scene->AddComponent(cameraEntity, Transform());

        scene->AddComponent(cameraEntity, Camera(vec2(applicationInstance->GetResolutionWidth(), applicationInstance->GetResolutionHeight())));

        mCameraSystem->CalculateProjections();

        directionalLight = scene->CreateEntity("Directional Light");

        scene->AddComponent(directionalLight, Transform());

        scene->AddComponent(directionalLight, DirectionalLight(
            glm::vec3(1.0f),
            glm::vec3(1.0f),
            glm::vec3(0.8f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            0.2f
        ));

    }

    void RenderSystem::SetCubemap(std::shared_ptr<Graphics::Cubemap> cubemap)
    {
        m_cubemap = cubemap;
    }

    void RenderSystem::Update()
    {
        Lynx::GameApplication* applicationInstance = Lynx::GameApplication::GetGameInstance();
        const auto& mCameraComponent = scene->GetComponent<Camera>(cameraEntity);
        const auto& mCameraTransform = scene->GetComponent<Transform>(cameraEntity);
        const auto& mDirLightComponent = scene->GetComponent<DirectionalLight>(directionalLight);
        const auto& mLightingSystem = applicationInstance->GetSystem<LightingSystem>();

        for (auto const& entity : entities) {
            const auto& mTransform = scene->GetComponent<Transform>(entity);
            const auto& mRenderComponent = scene->GetComponent<MeshRenderer>(entity);
            const auto& mMaterial = mRenderComponent->mat;

            if(!mRenderComponent->mesh){log_error("Render component not bind to a mesh"); continue;}

            mMaterial.shader->Use();
            
            if(mMaterial.shader == NULL){log_error("Invalid shader for entity %d!", entity); return;}

            mMaterial.shader->SetUniform("projection", mCameraComponent->Projection);
            mMaterial.shader->SetUniform("view", mCameraComponent->View);
            mMaterial.shader->SetUniform("model", mTransform->model);
            mMaterial.shader->SetUniform("color", mRenderComponent->ambient);
            mMaterial.shader->SetUniform("viewPos", mCameraTransform->position);

#ifdef LYNX_RENDER_DEBUG
            log_debug("\n--------------------------\n Render3D() :\nCamera nº%d \n Camera projection : %s\n Camera view : %s\n Camera position : %s\n Render Object : %d\n Texture ? %s\n Diffuse map %s\n Specular map ? %s\n Model matrix %s \n --------------------------", 
                cameraEntity,glm::to_string(mCameraComponent->Projection).c_str(),glm::to_string(mCameraComponent->View).c_str(),glm::to_string(mCameraTransform->position).c_str(), entity,
                "no", mMaterial.texture_diffuse->GetResourcePath().c_str(), "no", glm::to_string(mTransform->model).c_str());
#endif
            
			if(mRenderComponent->lighting | mLightingSystem->entities.size()){
				// Set lighting shader values
				int i = 0;
				for (auto lightEnt : mLightingSystem->entities){
					auto lightComponent = scene->GetComponent<PointLight>(lightEnt);
                    auto transform = scene->GetComponent<Transform>(lightEnt);
					char buffer[64];
                    
					sprintf(buffer, "pointLights[%d].position", i);
					mMaterial.shader->SetUniform(buffer , transform->position);
					
					sprintf(buffer, "pointLights[%d].constant", i);
					mMaterial.shader->SetUniform(buffer, lightComponent->constant);
					
					sprintf(buffer, "pointLights[%d].linear", i);
					mMaterial.shader->SetUniform(buffer, lightComponent->linear);
					
					sprintf(buffer, "pointLights[%d].quadratic", i);
					mMaterial.shader->SetUniform(buffer, lightComponent->quadratic);
					
					sprintf(buffer, "pointLights[%d].ambient", i);
					mMaterial.shader->SetUniform(buffer, lightComponent->ambient);

                    mMaterial.shader->SetUniform("material.ambient", mMaterial.ambient);
					mMaterial.shader->SetUniform("material.diffuse", mMaterial.diffuse);
                    mMaterial.shader->SetUniform("material.specular", mMaterial.specular);
                    mMaterial.shader->SetUniform("material.shininess", mMaterial.shininess);

                    
                    if(mMaterial.texture_diffuse){
                        mMaterial.shader->SetUniform("diffuse_map", true);
                        mMaterial.shader->SetUniform("material.diffuse_tex", 0);
                        mMaterial.texture_diffuse->Use();
                    }else{
                        mMaterial.shader->SetUniform("diffuse_map", false);
                    }

                    if(mMaterial.texture_specular){
                        mMaterial.shader->SetUniform("specular_map", true);
                        mMaterial.shader->SetUniform("material.specular_tex", 0);
                        mMaterial.texture_specular->Use();
                    }else{
                        mMaterial.shader->SetUniform("specular_map", false);
                    }
					i++;
				}
			}

            mMaterial.shader->SetUniform("directionalLight.direction", mDirLightComponent->direction);
            mMaterial.shader->SetUniform("directionalLight.ambient", mDirLightComponent->ambient);
            mMaterial.shader->SetUniform("directionalLight.diffuse", mDirLightComponent->diffuse);
            mMaterial.shader->SetUniform("directionalLight.specular", mDirLightComponent->specular);
            mMaterial.shader->SetUniform("directionalLight.intensity", mDirLightComponent->intensity);

            // Check if mesh has a texture, if so, render it
            if(mMaterial.texture){  
                mMaterial.texture->Use();
            }   

            mRenderComponent->mesh->VAO->Bind();
            mRenderComponent->mesh->Render();

            
        }


        if(m_cubemap)
            m_cubemap->Use(mCameraComponent->Projection, mCameraComponent->View);

    }

}
