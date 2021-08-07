#include "eventManager.h"

namespace Lynx {

std::map<EventType, std::vector<EventManager::EventFunc>> EventManager::listeners = std::map<EventType, std::vector<EventFunc>>();

}