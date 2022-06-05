#ifndef DEMO_HPP
#define DEMO_HPP

#include "Core/application.h"
#include "input.h"

class Demo : public Lynx::GameApplication {
public:
    Demo(int argc, char** argv);
    ~Demo();

    
private:
    static int OnUpdate(const Lynx::Event& ev); // Event listener for updating the application's title
    static char title[40];
    static Input input;
};

#endif