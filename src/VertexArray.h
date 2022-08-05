#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
private:
	unsigned int _rendererId;
	const void* offset;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const;

	void Bind() const;
	void Unbind() const;
};
