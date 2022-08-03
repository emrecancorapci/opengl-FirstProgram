#pragma once
#include <string>

class Texture
{
private:
	unsigned int _rendererID;
	std::string _filePath;
	unsigned char* _localBuffer;
	int _width, _height, _bytePerPixel;
public:
	Texture(std::string path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetHeight() const { return _height; }
	inline int GetWidth() const { return _width; }
};
