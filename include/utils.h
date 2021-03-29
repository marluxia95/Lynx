#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include "vertexBuffer.h"

using namespace std;

vector<Vertex> cube_vertices = {
    {
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 0.0f)
    },

    {
        vec3(1.0f, -1.0f, -1.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 0.0f, 0.0f)

    },
    {
        vec3(1.0f, 1.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 0.0f)
    },
    {
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 0.0f)
    },
    {
        vec3(-1.0f, -1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 0.0f)
    },
    {
        vec3(1.0f, -1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 0.0f)
    },
    {
        vec3(1.0f, 1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 0.0f)
    },
    {
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 0.0f)
    }
};

vector<GLuint> cube_indices = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};


#endif