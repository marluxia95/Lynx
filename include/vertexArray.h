#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

class VertexArray {
public:
	VertexArray(float* vertices);
	void Bind();
	void Unbind();
private:
	unsigned int VAO_ID;
};

#endif