#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

namespace Lynx {

    class Terrain : public Mesh {
    private:
        void SetupMesh();
    public:
        Terrain(float** heightmap);

        void Render();
    private:
        float** m_heightmap;

    };

}


#endif
