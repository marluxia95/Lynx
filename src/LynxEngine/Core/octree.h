#ifndef LYNX_OCTREE_H
#define LYNX_OCTREE_H

namespace Lynx {

    struct Octree {
        Octree* children;
        Octree* parent;

        unsigned char edges[12];
        unsigned char visible;
    };

    extern Octree* world_root;
    Octree* octree_create();
    void octree_subdivide(Octree* octree);
    void octree_free(Octree* octree);

}


#endif
