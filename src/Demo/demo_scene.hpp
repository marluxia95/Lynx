#pragma once

#include <memory>
#include <Systems/renderSystem.h>
#include "lynx.h"

extern Lynx::GameApplication* applicationInstance;

class DemoScene : public Lynx::Scene {
    public:
        DemoScene() : Lynx::Scene(applicationInstance->GetComponentManager()) {

        }

        void Init() override {
            auto resourceManager = applicationInstance->GetResourceManager();

            log_info("Adding scene objects");
            std::shared_ptr<Lynx::Graphics::Shader> shader = resourceManager->LoadShader("res/shaders/standard/lighting.vert", "res/shaders/standard/lighting.frag");

            Lynx::EntityID cube = Lynx::ModelLoader::loadModel(this, "res/models/plane.fbx", shader);
            {
                Lynx::MeshRenderer* meshRenderer = GetComponent<Lynx::MeshRenderer>(cube);
                Lynx::Transform* mTransform = GetComponent<Lynx::Transform>(cube);
                mTransform->rotation = glm::vec3(-90,0,0);
                mTransform->scale = glm::vec3(30.0);
                meshRenderer->ambient = glm::vec3(0.1f);
                meshRenderer->diffuse = glm::vec3(0.5f);
                meshRenderer->specular = glm::vec3(0.5f);
                meshRenderer->shininess = 24.0f;
                meshRenderer->texture_diffuse = resourceManager->LoadTexture("res/textures/wood.dds");
            }

            Lynx::EntityID lightEnt = CreateEntity("Light");
            AddComponent(lightEnt, Lynx::Transform{ glm::vec3(2,0,2), glm::vec3(0), glm::vec3(1) });
            AddComponent(lightEnt, Lynx::PointLight{ glm::vec3(0.4f, 0.7f , 0.4f ), glm::vec3(1.0f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f });
            

            auto directionalLight = GetComponent<Lynx::DirectionalLight>(applicationInstance->GetSystem<Lynx::RenderSystem>()->directionalLight);
            directionalLight->direction = glm::vec3(-1.0f, -0.5f, 0.0f);
            directionalLight->ambient = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->intensity = 1.0f;
        }

        void Destroy() override {

        }
};