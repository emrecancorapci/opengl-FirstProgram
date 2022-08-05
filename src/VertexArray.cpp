#include "VertexArray.h"
#include "Renderer.h"

#include <GL/glew.h>

VertexArray::VertexArray()
{

	glCall(glGenVertexArrays(1, &_rendererId));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &_rendererId));
}


void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const
{
	Bind();
	buffer.Bind();

	const auto& elements = layout.GetElements();
	const auto& size = layout.GetStride();

	for(unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		const void* offset;
		offset = reinterpret_cast<const void*>((i * element.count *
			LayoutElement::GetSizeOfType(element.type)));

		glCall(glEnableVertexAttribArray(i))
		glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, size, offset))
	}
}

void VertexArray::Bind() const
{
	glCall(glBindVertexArray(_rendererId));
}

void VertexArray::Unbind() const
{
	glCall(glBindVertexArray(0));
}
