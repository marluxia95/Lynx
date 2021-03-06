#ifndef LYNX_GRID_H
#define LYNX_GRID_H

#include "mesh.h"
#include "shader.h"
#include "lynx_common.h"

namespace Lynx::Graphics {

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