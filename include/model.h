#ifndef MODEL_H
#define MODEL_H
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include "vertexBuffer.h"

using namespace std;

namespace Lynx {

vector<Vertex>* loadModelFromFile(const char* path);

}


#endif