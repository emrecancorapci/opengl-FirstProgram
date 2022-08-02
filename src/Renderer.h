﻿#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


#define ASSERT(x) if(!(x)) __debugbreak();
#define glCall(x) glClearError();\
	x;\
	ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
	void Clear() const;
};
