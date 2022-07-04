#include "scene.h"

namespace Lynx {

    Scene::Scene()
    {

    }

    Scene::~Scene()
    {

    }

    Entity* Scene::CreateEntity()
    {
        Entity* newent = new Entity();
        m_entities.push_back(newent);
        return newent;
    }

    Entity* Scene::CreateEntity(const char* name)
    {
        Entity* newent = new Entity(name);
        m_entities.push_back(newent);
        return newent;
    }


}