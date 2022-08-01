#include "Renderer.h"

void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool glLogCall(const char* function, const char* file, int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "OpenGL Error: " << error << std::endl;
        std::cout << function << " in " << file << " at Line : " << line << std::endl;
        return false;
	}
    return true;
}