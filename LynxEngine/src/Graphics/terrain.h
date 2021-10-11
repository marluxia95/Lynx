#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
/*
namespace Lynx::Graphics {

    class Terrain : public Mesh{
    private:
        void SetupMesh();
    public:
        Terrain(float** heightmap);

        void Render();
    private:
        glm::vec3 calculate_normal(unsigned int x, unsigned int z);
        int getpos(unsigned int x, unsigned int z);
        float** m_heightmap;
        Vertex* vertices;
        unsigned int* indices;
        int m_rows, m_columns, m_vertcount, index_size;

    };

    float** load_heightmap_from_image(const char* path);

}
*/

#endif
