#include "Core/logger.h"
#include "Core/moduleInterface.h"

extern "C" { ENGINE_MODULE_INIT; ENGINE_MODULE_CLOSE; }

ENGINE_MODULE_INIT {
    log_info("Module test loaded!");
}

ENGINE_MODULE_CLOSE {
    log_info("Module test unloaded!");
}
