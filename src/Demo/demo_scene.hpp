#pragma once

#include <memory>
#include <Systems/renderSystem.h>
#include "lynx.h"
#include "Physics/physicsObject.h"

extern Lynx::GameApplication* applicationInstance;

class DemoScene : public Lynx::Scene {
    public:
        DemoScene() {
            log_debug("Created demo scene");
        }

        void Init() override {
            auto resourceManager = applicationInstance->GetResourceManager();

            log_info("Adding scene objects");
            std::shared_ptr<Lynx::Graphics::Shader> shader = resourceManager->LoadShader("res/shaders/standard/lighting.vert", "res/shaders/standard/lighting.frag");
            
            Lynx::ModelLoader loader(this);

            Lynx::Entity cube = loader.LoadModel("res/models/cube.fbx");

            Lynx::Graphics::Material mat(shader);
            mat.ambient = glm::vec3(0.1f);
            mat.diffuse = glm::vec3(0.5f);
            mat.specular = glm::vec3(0.5f);
            mat.shininess = 50.0f;
            mat.texture_diffuse = resourceManager->LoadTexture("res/textures/wood.dds");

            for(Lynx::EntityID child : cube.GetChildren()){
                if(!HasComponent<Lynx::MeshRenderer>(child))
                    continue;
                
                Lynx::MeshRenderer* cube_meshRenderer = GetComponent<Lynx::MeshRenderer>(child);
                Lynx::Transform* mTransform = GetComponent<Lynx::Transform>(child);
                mTransform->position = glm::vec3(0, 10.0f, 0);
                mTransform->scale = glm::vec3(1.0f);

                cube_meshRenderer->mat = mat;

                //AddComponent(cube, Lynx::PhysicsObject(1.0f));
            }

            Lynx::Entity floor = loader.LoadModel("res/models/plane.fbx");

            Lynx::Graphics::Material floor_mat(shader);
            floor_mat.ambient = glm::vec3(0.1f);
            floor_mat.diffuse = glm::vec3(0.5f);
            floor_mat.specular = glm::vec3(0.5f);
            floor_mat.shininess = 24.0f;
            floor_mat.texture_diffuse = resourceManager->LoadTexture("res/textures/box.dds");

            for(Lynx::EntityID child : floor.GetChildren()){
                if(!HasComponent<Lynx::MeshRenderer>(child))
                    continue;
                
                Lynx::MeshRenderer* meshRenderer = GetComponent<Lynx::MeshRenderer>(child);
                Lynx::Transform* mTransform = GetComponent<Lynx::Transform>(child);
                mTransform->position = glm::vec3(0,0,0);
                mTransform->rotation = glm::vec3(-90,0,0);
                mTransform->scale = glm::vec3(30.0);

                log_debug("%f", floor_mat.ambient.x);

                meshRenderer->mat = floor_mat;

                log_debug("%f", meshRenderer->mat.ambient.x);
            }
            

            Lynx::EntityID lightEnt = CreateEntity("Light");
            AddComponent(lightEnt, Lynx::Transform{ glm::vec3(2,1,0), glm::vec3(0), glm::vec3(1) });
            AddComponent(lightEnt, Lynx::PointLight( glm::vec3(0.4f, 0.7f , 0.4f ), glm::vec3(1.0f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f ));

            auto directionalLight = GetComponent<Lynx::DirectionalLight>(applicationInstance->GetSystem<Lynx::RenderSystem>()->directionalLight);
            directionalLight->direction = glm::vec3(-1.0f, -1.0f, 0.0f);
            directionalLight->ambient = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->intensity = 1.0f;

            {
                auto cubemapTexture = resourceManager->LoadTexture("res/textures/cubemap.dds", Lynx::Graphics::TEXTURE_CUBE_MAP);

                std::shared_ptr<Lynx::Graphics::Cubemap> cubemap = std::make_shared<Lynx::Graphics::Cubemap>(cubemapTexture);
                auto renderSystem = applicationInstance->GetSystem<Lynx::RenderSystem>();
                renderSystem->SetCubemap(cubemap);
                //auto shader = resourceManager->LoadShader("res/shaders/standard/cubemap.vert","res/shaders/standard/cubemap.frag");
            }


        }

        void Destroy() override {

        }
};