#include <glm/glm.hpp>
#include "renderer2D.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {

float quad_vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};

unsigned int quad_indices[] = {  
    0, 1, 3,             // first triangle
    1, 2, 3              // second triangle
};  

static VertexArray* quad_vao;
static VertexBuffer* quad_vbo;
static ElementBuffer* quad_ebo;

void Renderer2D::Init()
{
    quad_vbo = VertexBuffer::Create(&quad_vertices, sizeof(quad_vertices));

    quad_vao = VertexArray::Create();
    quad_vao->Bind();

    quad_vbo->Configure(MESH_2D_SPRITE);

    quad_ebo = ElementBuffer::Create(&quad_indices, sizeof(quad_indices)/sizeof(unsigned int));

    quad_vao->Unbind();

    API_CheckErrors();
}

void Renderer2D::RenderQuad()
{
    quad_ebo->Bind();
	RendererAPI::DrawIndexed(sizeof(quad_indices) / sizeof(unsigned int));
    API_CheckErrors();
}

void Renderer2D::Destroy()
{
    delete quad_vao, quad_vbo, quad_ebo;
}

}