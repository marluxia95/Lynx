#ifndef DEMO_H
#define DEMO_H

#include "Core/application.h"
#include "Core/scene.h"

class Demo : public Lynx::Application {
public:
    Demo(int argc, char** argv);
    ~Demo();

private:
    std::shared_ptr<Lynx::Scene> m_scene;
};

#endif