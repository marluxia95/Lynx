#ifndef LYNX_GRID_H
#define LYNX_GRID_H

#include "lynx_common.h"

namespace Lynx::Graphics {

    class Mesh;
    class Shader;

    class Grid {
    public:
        static void Create();
        static void Draw();
    private:
        static Graphics::Mesh*      grid_mesh;
        static Graphics::Shader*    grid_shader;
        static std::vector<Vertex>       grid_vertices;
        static std::vector<unsigned int> grid_indices;
    };  
}

#endif // LYNX_GRID_H
