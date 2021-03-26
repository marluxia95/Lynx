#include <GL/glew.h> 
#include "elementBuffer.h"

namespace Lynx {

ElementBuffer::ElementBuffer(const void* indices, unsigned int indexNumber){
    glGenBuffers(1, &ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNumber, indices, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer(){
    glDeleteBuffers(1, &ID);
}

void ElementBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

}