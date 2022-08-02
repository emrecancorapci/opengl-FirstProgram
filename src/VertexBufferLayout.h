#pragma once
#include <vector>

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406

struct LayoutElement
{
	unsigned int count;
	unsigned int type;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return sizeof(float);
			case GL_UNSIGNED_INT:	return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE:	return sizeof(unsigned char);
		}
		//ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
	std::vector<LayoutElement> _Elements;
	unsigned int _Stride;
public:
	VertexBufferLayout() : _Stride(0) {}

	template<typename T>
	void Push(const unsigned int& count);

	template<>
	void Push<float>(const unsigned int& count);

	template<>
	void Push<unsigned int>(const unsigned int& count);

	template<>
	void Push<unsigned char>(const unsigned int& count);

	inline std::vector<LayoutElement> GetElements() const { return _Elements; }
	inline unsigned int GetStride() const { return _Stride; }
};

