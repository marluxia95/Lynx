#include "event.h"

namespace Lynx {

    std::map<std::string, EventType> StringToEventMap = {
        {"None",                    None},
        {"WindowClose",             WindowClose},
        {"WindowResize",            WindowResize},
        {"KeyPressed",              KeyPressed},
        {"MousePosCallback",        MousePosCallback},
        {"MouseKeyPressed",         MouseKeyPressed},
        {"JoystickConnected",       JoystickConnected},
        {"JoystickDisconnected",    JoystickDisconnected},
        {"FirstTick",               FirstTick},
        {"LastTick",                LastTick},
        {"UpdateTick",              UpdateTick},
        {"EngineInit",              EngineInit},
        {"Render",                  Render},
        {"AsyncTextureLoad",        AsyncTextureLoad}
    };

    const char* GetEventString(EventType type)
    {
        for(auto &e : StringToEventMap) {
            if(e.second == type){
                return e.first.c_str();
            }
        }
    }

}