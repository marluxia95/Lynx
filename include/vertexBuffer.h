#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

class VertexBuffer{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	void Bind();
	void Unbind();
private:
	unsigned int VBO_ID;
};

#endif