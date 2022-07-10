#ifndef DEMO_H
#define DEMO_H

#include "Core/application.h"

class Demo : public Lynx::Application {
private:
    void movement();
public:
    Demo(int argc, char** argv);
    ~Demo();

private:
    Lynx::Camera* m_camera;
    float speed_mul;
    float sensitivity = 0.1f;
    bool mouse_active;
    glm::vec2 prev_pos;
    float pitch = 0;
    float yaw = 0;
};

#endif