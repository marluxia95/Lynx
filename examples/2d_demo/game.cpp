#include <stdio.h>
#include "lynx.h"

using namespace Lynx;

// Initialize global variables
ResourceManager gResourceManager;
Application gApplication;

class Render2DSystem : public ECS::System {
    public:
        void Init() 
        {
            log_debug("Render2DSystem : Init");

            Renderer2D::Init();

            // Automatically destroy renderer's data on last tick
            EventManager::AddListener(LastTick, [](const Event& ev){ Renderer2D::Destroy(); });

            cameraEntity = gApplication.CreateEntity("Main camera");
            const auto& mCameraSystem = gApplication.GetSystem<CameraSystem>();
            
            gApplication.AddComponent(cameraEntity, Transform{
                glm::vec3(0), 
                glm::vec3(0), 
                glm::vec3(0)
            });

            gApplication.AddComponent(cameraEntity, Camera{
                60, // Field of view
                vec2(gApplication.GetResolutionWidth(), gApplication.GetResolutionHeight()), // Resolution
                CAMERA_ORTHOGRAPHIC, // Camera type
                true, // Is it a main camera ?
                vec3(0.0f,1.0f,0.0f)   // Up vector
            });

            mCameraSystem->CalculateProjections();
        }

        void Update()
        {
            const auto& mCameraComponent = gApplication.GetComponent<Camera>(cameraEntity);
            const auto& mCameraTransform = gApplication.GetComponent<Transform>(cameraEntity);

            
            for (auto entity : entities) {
                auto transform_comp = gApplication.GetComponent<Transform>(entity);
                auto sprite_comp = gApplication.GetComponent<SpriteRenderer>(entity);
                auto shader = sprite_comp->shader;
                auto texture = sprite_comp->texture;

                if(!shader)
                    continue;
                
                if(texture)
                    texture->use();

                shader->use();
                mat4 model = transform_comp->GetModel();
            

                shader->setMat4("projection", mCameraComponent->projection);
                shader->setMat4("view", mCameraComponent->view);
                shader->setMat4("model", model);
                shader->setVec3("viewPos", mCameraTransform->position);
                
                Renderer2D::RenderQuad();
            }
        }

        Entity cameraEntity;
};

void RegisterData(const Event& ev)
{
    gApplication.LoadDefaultComponents();
	gApplication.LoadDefaultSystems();

    gApplication.RegisterComponent<SpriteRenderer>();

    gApplication.RegisterSystem<Render2DSystem>();
    {
        Signature signature;
        signature.set(gApplication.GetComponentType<Transform>());
        signature.set(gApplication.GetComponentType<SpriteRenderer>());
        gApplication.SetSystemSignature<Render2DSystem>(signature);
    }
}

int main(int argv, char** argc)
{
    log_set_level(LOG_DEBUG);

    EventManager::AddListener(EngineInit, RegisterData);

    gApplication.Init("2D Demo");

    Shader* shader = gResourceManager.LoadShader("Cube Shader", "res/shaders/standard/sprite.vs", "res/shaders/standard/sprite.fs");

    auto sprite_ent = gApplication.CreateEntity("Sprite");
    gApplication.AddComponent<Transform>(sprite_ent, Transform{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)});
    gApplication.AddComponent<SpriteRenderer>(sprite_ent, SpriteRenderer{shader, nullptr});


    gApplication.Run();
}