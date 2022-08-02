#pragma once
#include "Renderer.h"

#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::unordered_map<std::string, int> _uniformLocationCache;

	std::string _filePath;

	unsigned int _rendererId;

	ShaderProgramSource ParseShader(const std::string& shader) const;
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned type, const std::string& source);

	unsigned int GetUniformLocation(const std::string& name);


public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string&name, float x, float y, float z, float w);
	void SetUniform3f(const std::string&name, float x, float y, float z);
	void SetUniform2f(const std::string&name, float x, float y);
	void SetUniform1f(const std::string&name, float x);
	
};
