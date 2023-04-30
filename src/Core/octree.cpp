#include <cstdlib>
#include "logger.h"
#include "octree.h"

namespace Lynx {
	// TODO : Move this to the game's code instead of hard-coding it into the engine code.
    Octree* world_root = octree_create();
    int total_nodes = 0;

    Octree* octree_create()
    {
        Octree* oc = new Octree[8];
        for(int i = 0; i < 8; i++) {
            oc->parent = NULL;
            oc->children = NULL;

            oc->edges[0] = 0x80;
            oc->visible = 0;
            oc++;
        }
        total_nodes++;
        return oc-8;
    }

    void octree_subdivide(Octree* octree)
    {
        Octree* oc = octree->children = new Octree[8];
        for(int i = 0; i < 8; i++) {
            oc->parent = octree;
            oc->visible = 0;
            oc++;
        }
        total_nodes++;
    }

    void octree_free(Octree* octree)
    {
        //for(int i = 0; i < 8; i ++){
        log_debug("Freeing octree");
        if(octree->children) {
            Octree* o = octree->children;
            for(int i = 0; i < 8; i++) {
                log_debug("Freeing child");
                octree_free(o);
                o++;
            }
        }
        delete[] octree;
            //    octree++;
            //}
    }
}


