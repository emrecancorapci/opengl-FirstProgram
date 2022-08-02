#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>

void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool glLogCall(const char* function, const char* file, int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "OpenGL Error: " << error << std::endl;
        std::cout << function
				  << " in " << file
				  << " at Line : " << line << std::endl;
        return false;
	}
    return true;
}

void Renderer::Draw(const VertexArray& vertexArray,const IndexBuffer& indexBuffer,
					const Shader& shader) const
{
	shader.Bind();
	vertexArray.Bind();
    indexBuffer.Bind();

    glCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Clear() const
{
    glCall(glClear(GL_COLOR_BUFFER_BIT))
    glCall(glClearError())
}
