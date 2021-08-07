#include "vertexBuffer.h"

namespace Lynx::Graphics {

    void debugVBO(std::vector<Vertex>* vertices, std::vector<GLuint>* indices) {
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

}