#include <GL/glew.h> 
#include <iostream>
#include <vector>
#include "elementBuffer.h"

using namespace std;

namespace Lynx {

ElementBuffer::ElementBuffer(const void* indices, unsigned int indexNumber){
    glGenBuffers(1, &ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber, indices, GL_STATIC_DRAW);
}

ElementBuffer::ElementBuffer(vector<GLuint>* indices){
    glGenBuffers(1, &ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), &(indices->at(0)), GL_STATIC_DRAW );
    printf("bb -> %d\n", &(indices->at(0)));
}

ElementBuffer::~ElementBuffer(){
    glDeleteBuffers(1, &ID);
}

void ElementBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ElementBuffer::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

}