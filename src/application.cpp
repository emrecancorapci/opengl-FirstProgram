#include <GL/glew.h> //GLEW has to be included before GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 3
#define GL_PROFILE GLFW_OPENGL_CORE_PROFILE

#define RES_X 1600
#define RES_Y 900

// #define ASPECT_RATIO ((float)RES_X / (float)RES_Y)

// https://docs.gl/

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
static ShaderProgramSource ParseShader(const std::string& shader)
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
			if(line.find("vertex") != std::string::npos)
			{
				type = ShaderType::vertex;
			}
            else if(line.find("fragment") != std::string::npos)
            {
	            type = ShaderType::fragment;
            }
		}
        else
        {
	        ss[static_cast<int>(type)] << line << '\n';
        }
	}
    return { ss[0].str(), ss[1].str() };
}
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // Create the shader object
	glCall(unsigned int id = glCreateShader(type));
    // source string needs to be converted to const char*
	const char* src = source.c_str();
	// Load the shader source
	glCall(glShaderSource(id, 1, &src, nullptr));
	// Compile the shader
	glCall(glCompileShader(id));

    // Check for errors
	int result;
	glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;

        // glGetShaderiv takes a shader ID, and a parameter
        // that specifies what we want to know about the shader
        // in this case, the length of the error message.
		glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        // create a buffer of the correct size to store the error message
		// alloca() allocates memory on the stack,
		char* message = static_cast<char*>(_malloca(length * sizeof(char)));

        // get the error message
		glCall(glGetShaderInfoLog(id, length, &length, message));

        const std::string shaderType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
		std::cout << "Failed to compile " << shaderType << " shader!" << std::endl;
		std::cout << message << std::endl;

		glCall(glDeleteShader(id));
		return 0;
	}
	return id;
}
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    // Create and compile the vertex shader
    unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach the vertex shader to the program
    glCall(glAttachShader(program, vShader));
    glCall(glAttachShader(program, fShader));

    // Link the program
    glCall(glLinkProgram(program));

    // Validate the program
    glCall(glValidateProgram(program));

    return program;
}
//auto CreateTriangle(const float& width, const float& height, const float& x, const float& y)
//{
//    float triangle[] = {
//		(-(width/2) + x)/ASPECT_RATIO, -height/2 + y,
//		x/ASPECT_RATIO, height/2 + y,
//		(width/2 + x)/ASPECT_RATIO, -height/2 + y
//	};
//	
//	return triangle;
//}

int main(void)
{
    GLFWwindow* window;
                  
    /* Initialize the library */  
    if (!glfwInit())
    {
	    return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GL_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(RES_X, RES_Y, "Hello World",
        nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLEW
    if(glewInit() != GLEW_OK)
    {
	    return -1;
    }

    {
		const float positions[] = {
			-0.5f, -0.5f, // 0
			+0.5f, -0.5f, // 1
			+0.5f, +0.5f, // 2
			-0.5f, +0.5f  // 3
		};

	    const unsigned int indices[] = {
	        0, 1, 2,
	        2, 3, 0
	    };
		
	    // POSITION ARRAY BUFFER
		const VertexArray vertexArray;
		const VertexBuffer vertexBuffer(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2 floats for position
		vertexArray.AddBuffer(vertexBuffer, layout);


	    // INDEX ARRAY BUFFER
		const IndexBuffer indexBuffer(indices, 6);

	    unsigned int ibo;
	    int numberOfBuffers = 1;
	    int bufferDataType = GL_ELEMENT_ARRAY_BUFFER;
	    int usage = GL_STATIC_DRAW;

	    glCall(glGenBuffers(numberOfBuffers, &ibo));
	    glCall(glBindBuffer(bufferDataType, ibo));
	    glCall(glBufferData(bufferDataType, sizeof(unsigned int) * 3 * 2, indices, usage));

	    const ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		glCall(glUseProgram(shader));


	    glCall(const int uniformLocation = glGetUniformLocation(shader, "u_Color"));
	    ASSERT(uniformLocation != -1);

	    // Unbound everything
	    glCall(glBindVertexArray(0));
	    glCall(glUseProgram(0));
	    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	    float r = 0.0f;
	    float increment = 0.01f;
	    while (!glfwWindowShouldClose(window))
	    {
	        /* Render here */
	        glCall(glClear(GL_COLOR_BUFFER_BIT));
	        glCall(glClearError());

	        if(r > 1.0f) increment = -0.01f;
	        else if(r < 0.0f) increment = 0.01f;

	        r += increment;

    		glCall(glUseProgram(shader));
	        glCall(glUniform4f(uniformLocation, r, 0.0f, 0.3f, 1.0f));


			vertexArray.Bind();
    		indexBuffer.Bind();
	        
	        int numberOfIndices = 6;
	        int dataTypeOfIndices = GL_UNSIGNED_INT;
	        glCall(glDrawElements(GL_TRIANGLES, numberOfIndices, dataTypeOfIndices, nullptr));

	        /* Swap front and back buffers */
	        glCall(glfwSwapBuffers(window));

	        /* Poll for and process events */
	        glCall(glfwPollEvents());
	    }

		glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}
