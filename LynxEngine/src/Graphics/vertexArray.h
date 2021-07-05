#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

class VertexArray {
public:
	VertexArray();
	~VertexArray();
	void Bind();
	void Unbind();
	unsigned int VAO_ID;
};

#endif