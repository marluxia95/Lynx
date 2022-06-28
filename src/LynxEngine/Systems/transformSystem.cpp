#include "transformSystem.h"

namespace Lynx {
    void TransformSystem::Init() 
    {

    }

    void TransformSystem::Update() 
    {
        for ( auto const& entity : entities )
        {
            updateMatrices(entity);
		}
        
    }

    void TransformSystem::updateMatrices(Entity entity) 
    {
        //auto transform_component = entity.GetComponent<Transform>();
        
		//if(scene->HasComponent<Parent>(entity)) {
		//	EntityID parent = entity.GetComponent<Parent>()->parentEntity;
		//	transform_component->model = transform_component->CalculateModelMatrix() * scene->GetComponent<Transform>(parent)->model;
		//}else
		//transform_component->model = transform_component->CalculateModelMatrix();
    }
}