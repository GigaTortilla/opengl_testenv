#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <myshaders.h>
#include <testenv_funcs.h>
#include <cam.h>

// WARNING: #define STB_IMAGE_IMPLEMENTATION in main.c
#include <stb_image.h>

// Screen constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Camera cam = {
	.pos = {{ 0.0f, 0.0f, 4.0f }},
	.front = {{ 0.0f, 0.0f, -1.0f }},
	.right = {{ 1.0f, 0.0f, 0.0f }},
	.up = {{ 0.0f, 1.0f, 0.0f }},
	.speed = 0.05f,

	.firstMouse = true,
	.lastMouseX = 0.0f,
	.lastMouseY = 0.0f,

	.yawAngle = -90.0f,
	.pitchAngle = 0.0f
};

void mouseCallback(GLFWwindow* window, double xPos, double yPos);

int f_cubes()
{
	// Vertices and position array 
	// for displaying cubes
	float vertices[] = {
		// x y z coords		 // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	vec3 cubePositions[] = {
		{  0.0f,  0.0f,  0.0f },
		{  2.0f,  5.0f, -15.0f }, 
		{ -1.5f, -2.2f, -2.5f },  
		{ -3.8f, -2.0f, -12.3f },  
		{  2.4f, -0.4f, -3.5f },  
		{ -1.7f,  3.0f, -7.5f },  
		{  1.3f, -2.0f, -2.5f },  
		{  1.5f,  2.0f, -2.5f }, 
		{  1.5f,  0.2f, -1.5f }, 
		{ -1.3f,  1.0f, -1.5f }  
	};

	float lastFrame = 0.0f;

	GLFWwindow *window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "citrus.png", "OpenGLTestEnvironment");

	// hide and capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);

	unsigned int shaderProgram = buildShaderProgram("cubeShader.vert", "cubeShader.frag");
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute at attribute position 0 in the vertex shader layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Texture coords attribute at attribute position 1 in the vertex shader layout
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// unbind only when necessary
	// since VAOs require a call to glBindVertexArray to be modified anyways
	glBindVertexArray(0);
	
	// This block of code loads and binds the texture ressources
	stbi_set_flip_vertically_on_load(GL_TRUE);
	
	unsigned int texture1 = genBindTexRepeat("textures/container.jpg");
	unsigned int texture2 = genBindTexRepeat("textures/awesomeface.png");
	
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

	// Finding the location of the shader uniforms
	unsigned int modelLocation = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLocation = glGetUniformLocation(shaderProgram, "view");
	unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	unsigned int blendLocation = glGetUniformLocation(shaderProgram, "blend");

	//////////////////////
	/// Wireframe Mode ///
	//////////////////////
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Set to GL_FILL to display filled shapes or GL_LINE for wireframes
	
	// Render Loop
	while(!glfwWindowShouldClose(window))
	{
		checkESC(window);

		// Calculate delta time
		float timeValue = glfwGetTime();
		float deltaTime = timeValue - lastFrame;
		lastFrame = timeValue; 
		
		// clear screen first
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Bind the textures on the corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// render with the shader program and vertex array(s) 
		glUseProgram(shaderProgram);
		
		// Transformation matrices
		mat4 model, projection;
		glm_mat4_identity(model);
		glm_mat4_identity(projection);

		// Calculating the view matrix
		updateCam(&cam, window, deltaTime);
		mat4s view = glms_lookat(cam.pos, glms_vec3_add(cam.pos, cam.front), cam.up);
		
		// Transformation matrix calculations and sending them to the shader
		glm_perspective(glm_rad(45.0f), (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f, projection);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, *view.raw);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection[0]);

		float blendValue;

		glBindVertexArray(VAO);
		for(unsigned int i = 0; i < sizeof(cubePositions) / sizeof(vec3); i++)
		{
			glm_mat4_identity(model);
			glm_translate(model, cubePositions[i]);
			glm_rotate(model, glm_rad((20.0f * (i + 1.0f)) + 20.0f * ((i % 3) ? 1.0f : timeValue)), (vec3) { 1.0f, 0.3f, 0.5f });
			blendValue = (sin(2.5f * timeValue + i) + 1.0f) * 0.35f;

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model[0]);
			glUniform1f(blendLocation, blendValue);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Ressource deallocation
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	
	glfwTerminate();
	return 0;
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if(cam.firstMouse)
	{
		cam.lastMouseX = xPos;
		cam.lastMouseY = yPos;
		cam.firstMouse = false;
	}

	float offsetX = xPos - cam.lastMouseX;
	float offsetY = cam.lastMouseY - yPos;
	cam.lastMouseX = xPos;
	cam.lastMouseY = yPos;

	float sensitivity = 0.1f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	cam.yawAngle += offsetX;
	cam.pitchAngle += offsetY;

	if(cam.pitchAngle > 89.0f)
		cam.pitchAngle = 89.0f;
	if(cam.pitchAngle < -89.0f)
		cam.pitchAngle = -89.0f;
}