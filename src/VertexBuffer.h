#pragma once
#include "Renderer.h"

class VertexBuffer
{
private:
	unsigned int _rendererId;
public:
	VertexBuffer(const void* data, const unsigned int& size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
