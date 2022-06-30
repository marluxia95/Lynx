#include "demo.h"

Demo::Demo(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i) {
		if(std::string(argv[i]) == "--debug")
			log_set_level(LOG_DEBUG);
	}

    log_debug("test");

    Initialise(0);

    Run();
}

Demo::~Demo()
{
    
}