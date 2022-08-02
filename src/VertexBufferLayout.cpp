#include "VertexBufferLayout.h"

template <typename T>
void VertexBufferLayout::Push(const unsigned int& count)
{

}

template <>
void VertexBufferLayout::Push<float>(const unsigned int& count)
{
	_Elements.push_back(LayoutElement{count, GL_FLOAT, GL_FALSE});
	_Stride += count * LayoutElement::GetSizeOfType(GL_FLOAT);
}

template <>
void VertexBufferLayout::Push<unsigned int>(const unsigned int& count)
{
	_Elements.push_back(LayoutElement{count, GL_UNSIGNED_INT, GL_TRUE});
	_Stride += count * LayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
}
 
template <>
void VertexBufferLayout::Push<unsigned char>(const unsigned int& count)
{
	_Elements.push_back(LayoutElement{count, GL_UNSIGNED_BYTE, GL_TRUE});
	_Stride += count * LayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
