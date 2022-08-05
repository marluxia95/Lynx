#ifndef DEMO_H
#define DEMO_H

#include "Core/application.h"

class Demo : public Lynx::Application {
private:
    void movement();
    void spawn_cube();
public:
    Demo(int argc, char** argv);
    ~Demo();

private:
    int keystate = 0;
    Lynx::Camera* m_camera;
    float speed_mul;
    float sensitivity = 0.1f;
    glm::vec3 desired_position = glm::vec3(0.0f);
    glm::vec3 desired_rotation = glm::vec3(0.0f);
    bool mouse_active = false;
    bool mouse_start = false;
    glm::vec2 prev_pos = glm::vec2(0.0f);
    float pitch = 0;
    float yaw = 0;
};

#endif
