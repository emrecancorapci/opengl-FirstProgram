#include <GL/glew.h>
#include <iostream>

#include "vendor/stb_image/stb_image.h"
#include "Texture.h"
#include "Renderer.h"

Texture::Texture(std::string path)
: _rendererID(0), _filePath(std::move(path)), _localBuffer(nullptr),
_width(0), _height(0), _bytePerPixel(0)
{
	stbi_set_flip_vertically_on_load(1);
	_localBuffer = stbi_load(_filePath.c_str(),
		&_width, &_height, &_bytePerPixel, 4);

	glCall(glGenTextures(1, &_rendererID))
	glCall(glBindTexture(GL_TEXTURE_2D, _rendererID))

	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))

	if (_localBuffer)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(_localBuffer);
	}
	else
	{
		std::cout << "\nError: Failed to load texture" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
	}
}

Texture::~Texture()
{
	glCall(glDeleteTextures(1, &_rendererID))
}

void Texture::Bind(unsigned int slot) const
{
	glCall(glActiveTexture(GL_TEXTURE0 + slot))
	glCall(glBindTexture(GL_TEXTURE_2D, _rendererID))
}

void Texture::Unbind() const
{
	glCall(glBindTexture(GL_TEXTURE_2D, 0))
}
