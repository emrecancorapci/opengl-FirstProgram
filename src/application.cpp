#include <GL/glew.h> //GLEW has to be included before GLFW
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 3
#define GL_PROFILE GLFW_OPENGL_CORE_PROFILE

#define WINDOW_TITLE "OpenGL 3.3"
#define RESOLUTION_X 1600
#define RESOLUTION_Y 900

// #define ASPECT_RATIO ((float)RES_X / (float)RES_Y)

int main(void)
{
    GLFWwindow* window;
	
    if (!glfwInit())	return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GL_PROFILE);


    window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y,
		WINDOW_TITLE, nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
	
    if(glewInit() != GLEW_OK)	return -1;

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

		const VertexArray vertexArray;
		const VertexBuffer vertexBuffer(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2 floats for each position
		vertexArray.AddBuffer(vertexBuffer, layout);

		const IndexBuffer indexBuffer(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
    	shader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.3f, 1.0f);

		vertexArray.Unbind();
    	vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

	    float r = 0.0f;
	    float increment = 0.01f;
	    while (!glfwWindowShouldClose(window))
	    {
	        glCall(glClear(GL_COLOR_BUFFER_BIT));
	        glCall(glClearError());

	        if(r > 1.0f) increment = -0.01f;
	        else if(r < 0.0f) increment = 0.01f;

	        r += increment;

    		shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.0f, 0.3f, 1.0f);

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
    }
    glfwTerminate();
    return 0;
}
