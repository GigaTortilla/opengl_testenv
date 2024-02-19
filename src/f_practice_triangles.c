#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <myshaders.h>
#include <testenv_funcs.h>

int f_practice_triangle()
{
	// Initialize and configure GLFW
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// Tells GLFW that we're using OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// required under Mac OS X
#endif
	
	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Somehow setting the viewport here after window creation crashes the application
	// glViewport(0, 0, 800, 600);		// Sets the viewport to the lower left corner with size 800x600
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// Initialize and load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return -1;
	}
	
	////////////////////////////////////////////
	/// Build and compile the shader program ///
	////////////////////////////////////////////
	unsigned int shaderProgramOrange = buildShaderProgram("triangle.vert", "orange_tri.frag");
	unsigned int shaderProgramYellow = buildShaderProgram("triangle.vert", "yellow_tri.frag");
	
	// Example data 
	float leftTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
	};
	float rightTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
	};
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangle), leftTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// no need to unbind since we're directly binding a different VAO the next few lines
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangle), rightTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// unbind only when necessary
	// since VAOs require a call to glBindVertexArray to be modified anyways
	glBindVertexArray(0);
	
	//////////////////////
	/// Wireframe Mode ///
	//////////////////////
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Set to GL_FILL to display filled shapes or GL_LINE for wireframes
	
	// Render Loop
	while(!glfwWindowShouldClose(window))
	{
		checkESC(window);
		
		// clear screen first
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// render with the shader program and vertex array(s) 
		glUseProgram(shaderProgramOrange);
		
		// change the color of the first triangle
		float timeValue = glfwGetTime();
		float redValue = sin(3 * timeValue) / 2.0f + 0.5f;
		int leftTriangleColorLocation = glGetUniformLocation(shaderProgramOrange, "ourColor");
		glUniform4f(leftTriangleColorLocation, redValue, 0.65f, 0.0f, 1.0f);
		
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glUseProgram(shaderProgramYellow);
		
		// Change the color of the second triangle
		int rightTriangleColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
		float greenValue = 1.0f - redValue;
		glUniform4f(rightTriangleColorLocation, 1.0f, greenValue, 0.0f, 1.0f);		
		
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Ressource deallocation
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgramOrange);
	glDeleteProgram(shaderProgramYellow);
	
	glfwTerminate();
	return 0;
}
