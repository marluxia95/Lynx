#include "Core/scene.h"
#include "Core/event_manager.h"
#include "Graphics/renderer_forward.h"
#include "Graphics/model.h"
#include "demo.h"


using namespace Lynx;

Demo::Demo(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i) {
		if(std::string(argv[i]) == "--debug")
			log_set_level(LOG_DEBUG);
	}

    log_debug("test");

    m_scene.reset(new Scene());

    Initialise(0);
    m_renderer.reset(new Graphics::ForwardRenderer());
    m_renderer->Initialise();

    Entity* model;

    {
        Graphics::ModelLoader loader(m_scene);
        model = loader.LoadModel("res/models/cube.fbx");
    }

    m_renderer->PushRender(model);

    Run();
}

Demo::~Demo()
{
    
}