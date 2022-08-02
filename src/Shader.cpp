#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Renderer.h"
#include "Shader.h"

Shader::Shader(const std::string& filePath) : _filePath(filePath), _rendererId(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	_rendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glCall(glDeleteProgram(_rendererId));
}

ShaderProgramSource Shader::ParseShader(const std::string& shader) const
{
	std::ifstream stream(shader);

    enum class ShaderType
	{
		none = -1,
		vertex = 0,
		fragment = 1
	};

    ShaderType type = ShaderType::none;
    std::string line;
    std::stringstream ss[2];

    while(getline(stream, line))
	{
		if(line.find("#shader") != std::string::npos)
		{
			type = line.find("vertex") != std::string::npos	?
						ShaderType::vertex:
				   line.find("fragment") != std::string::npos ?	
						ShaderType::fragment:
						ShaderType::none;
		}
        else
        {
	        ss[static_cast<int>(type)] << line << '\n';
        }
	}
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	const unsigned int program = glCreateProgram();

	const unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	const unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


    glCall(glAttachShader(program, vShader))
    glCall(glAttachShader(program, fShader))

    glCall(glLinkProgram(program))
    glCall(glValidateProgram(program))

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	glCall(const unsigned int id = glCreateShader(type))
	const char* src = source.c_str();

	glCall(glShaderSource(id, 1, &src, nullptr))
	glCall(glCompileShader(id))


	int result;
	glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result))
	if (result == GL_FALSE)
	{
		int length;
		glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length))
		
		char* message = static_cast<char*>(_malloca(length * sizeof(char)));
		glCall(glGetShaderInfoLog(id, length, &length, message))

        const std::string shaderType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
		std::cout << "Failed to compile " << shaderType << " shader!" << std::endl;
		std::cout << message << std::endl;

		glCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

void Shader::Bind() const
{
	glCall(glUseProgram(_rendererId))
}

void Shader::Unbind() const
{
	glCall(glUseProgram(0))
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w)
{
	glCall(glUniform4f(GetUniformLocation(name), x, y, z, w))
}

void Shader::SetUniform3f(const std::string& name, float x, float y, float z)
{
	glCall(glUniform3f(GetUniformLocation(name), x, y, z))
}

void Shader::SetUniform2f(const std::string& name, float x, float y)
{
	glCall(glUniform2f(GetUniformLocation(name), x, y))
}

void Shader::SetUniform1f(const std::string& name, float x)
{
	glCall(glUniform1f(GetUniformLocation(name), x))
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if(_uniformLocationCache.contains(name)) return _uniformLocationCache[name];

	glCall(const int location = glGetUniformLocation(_rendererId, name.c_str()))

	if(location == -1)
	{
		std::cout << "Uniform " << name << " not found!" << std::endl;
	}

	return location;
}
