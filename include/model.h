#ifndef MODEL_H
#define MODEL_H
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include "vertexBuffer.h"

using namespace std;

class Model {
    public:
    bool load(const char* path);

    vector<Vertex> vertices;
    vector<GLuint> vert_indices;
    vector<GLuint> tex_indices;
    vector<GLuint> normal_indices;
    
    

};


#endif