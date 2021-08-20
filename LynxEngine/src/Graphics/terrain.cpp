#include "terrain.h"

namespace Lynx {

    Terrain::Terrain(float** heightmap)
    {
        m_heightmap = heightmap;

        VAO = new VertexArray();
        VAO->Bind();

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void Terrain::SetupMesh()
    {

    }
}
