#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : _count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint))
    
	glCall(glGenBuffers(1, &_rendererId))
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId))
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW))
}

IndexBuffer::~IndexBuffer()
{
    glCall(glDeleteBuffers(1, &_rendererId))
}

void IndexBuffer::Bind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId))
}

void IndexBuffer::Unbind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0))
}
