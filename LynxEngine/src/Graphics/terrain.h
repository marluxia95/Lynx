#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"

namespace Lynx::Graphics {

    class LYNXENGINE_API Terrain : public Mesh{
    private:
        void SetupMesh();
    public:
        Terrain(std::vector<float> heightmap);
        Terrain(const char* image);
        ~Terrain();

    private:
        glm::vec3 calculate_normal(unsigned int x, unsigned int z);
        int getpos(unsigned int x, unsigned int z);
        void load_heightmap_from_image(const char* path);
    
    private:

        std::vector<float> m_heightmap;
        
        int gridlen;
        int m_rows, m_columns, m_vertcount, index_size;

    };

}


#endif
