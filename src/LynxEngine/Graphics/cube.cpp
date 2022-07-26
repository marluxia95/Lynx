#include "cube.h"

namespace Lynx::Graphics {

	Cube::Cube() : Mesh()
	{
		// Pre-generated mesh dump
		vertices = new std::vector<Vertex>{
			Vertex{glm::vec3(1.000000,	1.000000,1.000000), 	glm::vec3(0.000000,0.000000,1.000000), 	glm::vec2(0.999522, 1.000031)},
			Vertex{glm::vec3(-1.000000,	1.000000,1.000000), 	glm::vec3(0.000000,0.000000,1.000000),	glm::vec2(1.998565, 1.000031)},
			Vertex{glm::vec3(-1.000000,-1.000000,1.000000), 	glm::vec3(0.000000,0.000000,1.000000), 	glm::vec2(1.998565, 0.000988)},
			Vertex{glm::vec3(1.000000, -1.000000,1.000000), 	glm::vec3(0.000000,0.000000,1.000000), 	glm::vec2(0.999522, 0.000988)},
			Vertex{glm::vec3(1.000000, -1.000000,-1.000000), 	glm::vec3(0.000000,-1.000000,0.000000), glm::vec2(0.000478, 0.000988)},
			Vertex{glm::vec3(1.000000, -1.000000,1.000000), 	glm::vec3(0.000000,-1.000000,0.000000), glm::vec2(0.999522, 0.000988)},
			Vertex{glm::vec3(-1.000000,-1.000000,1.000000), 	glm::vec3(0.000000,-1.000000,0.000000), glm::vec2(0.999522,-0.998055)},
			Vertex{glm::vec3(-1.000000,-1.000000,-1.000000), 	glm::vec3(0.000000,-1.000000,0.000000), glm::vec2(0.000478,-0.998055)},
			Vertex{glm::vec3(-1.000000,-1.000000,-1.000000), 	glm::vec3(-1.000000,0.000000,0.000000), glm::vec2(0.000478, 2.998118)},
			Vertex{glm::vec3(-1.000000,-1.000000,1.000000), 	glm::vec3(-1.000000,0.000000,0.000000), glm::vec2(0.999522, 2.998118)},
			Vertex{glm::vec3(-1.000000, 1.000000,1.000000), 	glm::vec3(-1.000000,0.000000,0.000000), glm::vec2(0.999522, 1.999075)},
			Vertex{glm::vec3(-1.000000, 1.000000,-1.000000), 	glm::vec3(-1.000000,0.000000,0.000000), glm::vec2(0.000478, 1.999075)},
			Vertex{glm::vec3(-1.000000, 1.000000,-1.000000), 	glm::vec3(0.000000,0.000000,-1.000000), glm::vec2(-0.998565,1.000031)},
			Vertex{glm::vec3(1.000000,  1.000000,-1.000000), 	glm::vec3(0.000000,0.000000,-1.000000), glm::vec2(0.000478, 1.000031)},
			Vertex{glm::vec3(1.000000, -1.000000,-1.000000), 	glm::vec3(0.000000,0.000000,-1.000000), glm::vec2(0.000478, 0.000988)},
			Vertex{glm::vec3(-1.000000,-1.000000,-1.000000), 	glm::vec3(0.000000,0.000000,-1.000000), glm::vec2(-0.998565,0.000988)},
			Vertex{glm::vec3(1.000000,  1.000000,-1.000000), 	glm::vec3(1.000000,0.000000,0.000000), 	glm::vec2(0.000478, 1.000031)},
			Vertex{glm::vec3(1.000000,  1.000000,1.000000), 	glm::vec3(1.000000,0.000000,0.000000), 	glm::vec2(0.999522, 1.000031)},
			Vertex{glm::vec3(1.000000, -1.000000,1.000000), 	glm::vec3(1.000000,0.000000,0.000000), 	glm::vec2(0.999522, 0.000988)},
			Vertex{glm::vec3(1.000000, -1.000000,-1.000000), 	glm::vec3(1.000000,0.000000,0.000000), 	glm::vec2(0.000478, 0.000988)},
			Vertex{glm::vec3(-1.000000, 1.000000,-1.000000), 	glm::vec3(0.000000,1.000000,0.000000), 	glm::vec2(0.000478, 1.999075)},
			Vertex{glm::vec3(-1.000000, 1.000000,1.000000), 	glm::vec3(0.000000,1.000000,0.000000), 	glm::vec2(0.999522, 1.999075)},
			Vertex{glm::vec3(1.000000,  1.000000,1.000000), 	glm::vec3(0.000000,1.000000,0.000000), 	glm::vec2(0.999522, 1.000031)},
			Vertex{glm::vec3(1.000000,  1.000000,-1.000000), 	glm::vec3(0.000000,1.000000,0.000000), 	glm::vec2(0.000478, 1.000031)}
		};

		indices = new std::vector<unsigned int>{
			0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23
		};

        type = MESH_3D_TEXTURED_NORMAL;

        log_debug("Vertex count %d Index count %d", vertices->size(), indices->size());
		VBO = VertexBuffer::Create(vertices);

		VAO = VertexArray::Create();
		VAO->Bind();

		VBO->Configure(type);

		EBO = ElementBuffer::Create(indices);

		VAO->Unbind();
	}

}
