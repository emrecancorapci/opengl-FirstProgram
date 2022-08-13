#include <GL/glew.h> //GLEW has to be included before GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"



#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 3
#define GL_PROFILE GLFW_OPENGL_CORE_PROFILE

#define WINDOW_TITLE "OpenGL 3.3"
#define IRESOLUTION_X 1600
#define IRESOLUTION_Y 900
#define FRESOLUTION_X 1600.0f
#define FRESOLUTION_Y 900.0f

int main(void)
{
    GLFWwindow* window;
	
    if (!glfwInit())	return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GL_PROFILE);

    window = glfwCreateWindow(IRESOLUTION_X, IRESOLUTION_Y,
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
		//	posx,posy,texCoordinateX,texCoordinateY
		const float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, // 0
			+0.5f, -0.5f, 1.0f, 0.0f, // 1
			+0.5f, +0.5f, 1.0f, 1.0f, // 2
			-0.5f, +0.5f, 0.0f, 1.0f // 3
		};

	    const unsigned int indices[] = {
	        0, 1, 2,
	        2, 3, 0
	    };

		glCall(glEnable(GL_BLEND))
		glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
		

		VertexArray vertexArray;
		VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2 floats for each position
		layout.Push<float>(2); // 2 floats for each texture coordinate
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(indices, 6);

		const glm::mat4 mvpMatrix = glm::ortho(
			-FRESOLUTION_X / 1000, FRESOLUTION_X / 1000,
			-FRESOLUTION_Y / 1000, FRESOLUTION_Y / 1000,
			-1.0f, 1.0f);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
    	shader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.3f, 1.0f);
		shader.SetUniformMat4f("u_modelViewProjectionMatrix", mvpMatrix);
		

		Texture texture("res/textures/glass-bin.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		vertexArray.Unbind();
    	vertexBuffer.Unbind();
		indexBuffer.Unbind();
		shader.Unbind();

		Renderer renderer;

	    float r = 0.0f;
	    float increment = 0.01f;
	    while (!glfwWindowShouldClose(window))
	    {
			renderer.Clear();

	        if(r > 1.0f) increment = -0.01f;
	        else if(r < 0.0f) increment = 0.01f;
			
	        r += increment;

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.0f, 0.3f, 1.0f);

	        renderer.Draw(vertexArray,indexBuffer, shader);

	        /* Swap front and back buffers */
	        glCall(glfwSwapBuffers(window));

	        /* Poll for and process events */
	        glCall(glfwPollEvents());
	    }
    }
    glfwTerminate();
    return 0;
}
