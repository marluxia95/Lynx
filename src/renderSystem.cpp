#include <stdio.h>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "simpleGameEngine.h"
#include "components.h"
#include "renderSystem.h"


namespace Lynx {
    
    extern Core::Game game;

    void RenderSystem::Init(){

    }

    void RenderSystem::Update(){
    
        for (auto const& entity : entities) {
            const auto mTransform = game.GetComponent<Transform>(entity);
            const auto mRenderComponent = game.GetComponent<MeshRenderer>(entity);

            mRenderComponent.mesh->VAO->Bind();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRenderComponent.mesh->EBO);
            glDrawElements(GL_TRIANGLES, mRenderComponent.mesh->indices->size(), GL_UNSIGNED_INT, (void*)0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

    }

}