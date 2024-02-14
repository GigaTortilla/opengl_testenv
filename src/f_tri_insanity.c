#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <myshaders.h>
#include <testenv_funcs.h>

int f_tri_insanity()
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
	unsigned int shaderProgramTri = buildShaderProgram("shader.vert", "shader.frag");
	
	// Example data 
	float vertices[] = {
		// positions         // colors
    	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
    	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute at attribute position 0 in the vertex shader layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Color attribute at attribute position 1 in the vertex shader layout
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
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
		processInput(window);
		
		// clear screen first
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// render with the shader program and vertex array(s) 
		glUseProgram(shaderProgramTri);
		
		// change the color of the triangle
		float timeValue = glfwGetTime();
		float offsetValue = sin(3 * timeValue) / 3.0f;
		int triangleOffsetLocation = glGetUniformLocation(shaderProgramTri, "ourOffsetX");
		glUniform1f(triangleOffsetLocation, offsetValue);
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Ressource deallocation
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgramTri);
	
	glfwTerminate();
	return 0;
}
