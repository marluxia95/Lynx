#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include <iostream>
#include <vector>
#include <GL/glew.h>

using namespace std;

namespace Lynx {

class ElementBuffer {

    public:
        ElementBuffer(const void* indices, unsigned int indexNumber);
        ElementBuffer(vector<GLuint>* indices);
        ~ElementBuffer();
        void Bind();
        void Unbind();

    private:
        unsigned int ID;

};

}

#endif