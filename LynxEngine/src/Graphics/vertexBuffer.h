#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/logger.h"
#include "mesh.h"

using namespace glm;

namespace Lynx::Graphics {

    enum MeshType : unsigned int;

    struct Vertex {
        vec3 Position;
        vec2 TextureCoords;
        vec3 Normal;
    };

    void debugVBO(std::vector<Vertex>* vertices, std::vector<GLuint>* indices)
    {
        for ( int i = 0; i < indices->size(); i++ ){
            log_debug("Index Number %d Vertex pos: %f %f %f Texture Coord pos %f %f Normal pos %f %f %f\n", i, 
            vertices->at(indices->at(i)).Position.x, 
            vertices->at(indices->at(i)).Position.y, 
            vertices->at(indices->at(i)).Position.z,
            vertices->at(indices->at(i)).TextureCoords.x,
            vertices->at(indices->at(i)).TextureCoords.y,
            vertices->at(indices->at(i)).Normal.x,
            vertices->at(indices->at(i)).Normal.y,
            vertices->at(indices->at(i)).Normal.z);
        }
    }

    class VertexBuffer {
        public:
            virtual void Configure(MeshType type) = 0;
            virtual void Bind() = 0;
            virtual void Unbind() = 0;
            unsigned int VBO_ID;
    };

}

#endif