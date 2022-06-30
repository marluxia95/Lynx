#include "scene.h"

namespace Lynx {

    Scene::Scene()
    {

    }

    Scene::~Scene()
    {

    }

    EntityHandlePtr Scene::CreateEntity()
    {
        return std::make_shared<Entity>();
    }

    EntityHandlePtr Scene::CreateEntity(const char* name)
    {
        return std::make_shared<Entity>(name);
    }


}