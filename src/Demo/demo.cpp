#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Core/entity_manager.h"
#include "Core/input.h"
#include "Core/event_manager.h"
#include "Core/module.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"
#include "Graphics/renderer_forward.h"
#include "Graphics/model.h"
#include "Graphics/skybox.h"
#include "Physics/physics_object.h"
#include "demo.h"

using namespace Lynx;

Demo::Demo(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i) {
		if(std::string(argv[i]) == "--debug")
			log_set_level(LOG_DEBUG);
	}

    Initialise(0);

    m_renderer.reset(new Graphics::ForwardRenderer());
    m_renderer->Initialise();

    m_camera = new Camera();
    m_camera->CalcPerspective(GetResolutionWidth(), GetResolutionHeight(), 0.1f, 1000.0f);
    m_camera->position = glm::vec3(-15, 15, 0);
    m_camera->rotation = glm::vec3(0, 90, 0);

    m_renderer->SetCamera(m_camera);
    auto directional_light = Graphics::DirectionalLight{glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.4f), glm::vec3(1.0f)};
    m_renderer->SetDirectionalLight(directional_light);

    Graphics::ModelLoader loader(m_entityManager);

    Entity* plane;
    {
        plane = loader.LoadModel("res/models/plane.fbx");
        plane->SetLocalScaling(glm::vec3(10.0f));
        plane->SetLocalRotation(glm::vec3(-90,0,0));
        Graphics::Material plane_material;
        plane_material.texture_diffuse = m_resourceManager->LoadTexture("res/textures/wood.dds");
        plane_material.ambient = glm::vec3(0.5f);
        plane_material.diffuse = plane_material.specular = glm::vec3(0.5f);
        plane_material.shininess = 20.0f;

        plane->GetChildByIndex(0)->GetRenderHndl()->SetMaterial(plane_material);
        log_debug("%s %s %s", glm::to_string(plane->GetGlobalPosition()).c_str(), glm::to_string(plane->GetGlobalRotation()).c_str(),
                glm::to_string(plane->GetGlobalScaling()).c_str());
    }

    std::shared_ptr<Graphics::Skybox> sky = std::make_shared<Graphics::Skybox>(m_resourceManager->LoadTexture("res/textures/cubemap.dds", Graphics::TEXTURE_CUBE_MAP));
    m_renderer->SetSkybox(sky);

    EventManager::AddListener(MouseKeyPressed, [this](const Event& ev){
        const MouseButtonEvent& button_event = static_cast<const MouseButtonEvent&>(ev);
        if(button_event.m_keyCode == MOUSE_BUTTON_2){
            mouse_active = button_event.m_action;
            Input::EnableCursor(mouse_active);
        }

    });

    EventManager::AddListener(UpdateTick, [this](const Event& ev){
        movement();
        if(Input::IsKeyDown(KEY_C) && !keystate){
            spawn_cube();
            keystate = 1;
        }else{
            keystate = 0;
        }
    });

    desired_position = m_camera->position;
    desired_rotation = m_camera->rotation;

    Run();
}

Demo::~Demo()
{

}

void Demo::spawn_cube()
{
    auto loader = Graphics::ModelLoader(m_entityManager);
    Entity* cube;
    cube = loader.LoadModel("res/models/cube.fbx");

    cube->SetLocalPosition(glm::vec3(0,10,0));

    Graphics::Material cube_material;
    cube_material.texture_diffuse = m_resourceManager->LoadTexture("res/textures/box.dds");
    cube_material.ambient = glm::vec3(0.5f);
    cube_material.diffuse = cube_material.specular = glm::vec3(0.5f);
    cube_material.shininess = 50.0f;
    cube->GetChildByIndex(0)->GetRenderHndl()->SetMaterial(cube_material);

    Physics::PhysicsObject* cube_phys = new Physics::PhysicsObject();
    cube_phys->SetLinearVelocity(glm::vec3(0, 0.5f, 0.5f));
    cube->SetPhysicsObj(cube_phys);
}

void Demo::movement()
{
    float speed = GetDeltaTime() * speed_mul;
    float forward = Input::IsKeyDown(KEY_W) - Input::IsKeyDown(KEY_S);
    float left = Input::IsKeyDown(KEY_D) - Input::IsKeyDown(KEY_A);
    if(forward || left)
        desired_position = m_camera->position + speed * (m_camera->rotation * forward + glm::normalize(glm::cross(m_camera->rotation, m_camera->Up())) * left );

    glm::vec3 rate(0.1f);
    m_camera->position = glm::lerp(m_camera->position, desired_position, rate);
    speed_mul = 30.0f + Input::IsKeyDown(KEY_LEFT_SHIFT) * 30.0f;

    if(!mouse_active) {
        return;
    }

    glm::vec2 pos = Lynx::Input::GetMousePos();
    glm::vec2 offset = glm::vec2(pos.x - prev_pos.x, prev_pos.y - pos.y);
    prev_pos = pos;


    offset *= sensitivity;

    pitch += offset.y;
    yaw += offset.x;

    if(pitch > 89.9f)
        pitch = 89.9f;

    if(pitch < -89.9f)
        pitch = -89.9f;

    //log_debug("p%f y%f pos(%f %f) ppos(%f %f)", pitch, yaw, pos.x, pos.y, prev_pos.x, prev_pos.y);

    desired_rotation = glm::normalize(
        glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch) )));

    rate = glm::vec3(0.5f);
    m_camera->rotation = glm::lerp(m_camera->rotation, desired_rotation, rate);
}
