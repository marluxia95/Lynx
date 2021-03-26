#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include <GL/glew.h> 

namespace Lynx {

class ElementBuffer {

    public:
        ElementBuffer(const void* indices, unsigned int indexNumber);
        ~ElementBuffer();
        void Bind();

    private:
        unsigned int ID;

};

}

#endif