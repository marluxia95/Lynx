#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h> 
  
#include <stdio.h>

class Texture {
public:
	Texture(const char* path);
	void use();
	void Destroy();
	unsigned int texture = 0;
	int id;

};


#endif

