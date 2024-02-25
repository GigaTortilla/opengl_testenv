#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <myshaders.h>
#include <cam.h>
#include <testenv_funcs.h>

// WARNING: #define STB_IMAGE_IMPLEMENTATION in main.c
#include <stb_image.h>

// Screen constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Global camera struct initialization
Camera cam_light = {
	.pos = {{ 0.0f, 0.0f, 4.0f }},
	.front = {{ 0.0f, 0.0f, -1.0f }},
	.right = {{ 1.0f, 0.0f, 0.0f }},
	.up = {{ 0.0f, 1.0f, 0.0f }},

	.fov = 45.0f,
	.speed = 0.05f,

	.firstMouse = true,
	.lastMouseX = SCREEN_WIDTH / 2.0f,
	.lastMouseY = SCREEN_HEIGHT / 2.0f,

	.yawAngle = -90.0f,
	.pitchAngle = 0.0f
};

void mouseCallbackLight(GLFWwindow* window, double xPos, double yPos);
void scrollCallbackLight(GLFWwindow* window, double xoffset, double yoffset);

int f_lighting()
{
	// Vertices array 
	// Serves as data for displaying cubes. 
	float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

	// Stores the time at the previous frame to calculate time differences between frames
	float lastFrame = 0.0f;

	GLFWwindow *window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "citrus.png", "OpenGLTestEnvironment");

	// hide and capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallbackLight);
	glfwSetScrollCallback(window, scrollCallbackLight);

	// Build the shader programs
	// Since both programs use the same functions in the vertex shader stage, using the same vertex shader file is a good option
	unsigned int colorShaderProgram = buildShaderProgram("colorShader.vert", "colorShader.frag");
	unsigned int lightShaderProgram = buildShaderProgram("colorShader.vert", "lightCube.frag");
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBO, cubeVAO, lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(cubeVAO);
	// Position attribute at attribute position 0 in the vertex shader layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Repeat this process for the lightVAO 
	// We only need to bind the VBO again to set glVertexAttribPointer
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Position attribute at attribute position 0 in the vertex shader layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Finding the location of the shader uniforms

	unsigned int cubeModelLocation = glGetUniformLocation(colorShaderProgram, "model");
	unsigned int cubeViewLocation = glGetUniformLocation(colorShaderProgram, "view");
	unsigned int cubeProjectionLocation = glGetUniformLocation(colorShaderProgram, "projection");

	unsigned int lightModelLocation = glGetUniformLocation(lightShaderProgram, "model");
	unsigned int lightViewLocation = glGetUniformLocation(lightShaderProgram, "view");
	unsigned int lightProjectionLocation = glGetUniformLocation(lightShaderProgram, "projection");

	unsigned int objectColorLocation = glGetUniformLocation(colorShaderProgram, "objectColor");
	unsigned int lightColorLocation = glGetUniformLocation(colorShaderProgram, "lightColor");
	
	// lighting
	vec3 lightPos = { 1.2f, 1.0f, 2.0f };
	vec3 objectColor = { 1.0f, 0.5f, 0.31f };
	vec3 lightColor = { 1.0f, 1.0f, 1.0f };

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// render with the shader program and vertex array(s) 
		glUseProgram(colorShaderProgram);

		// Set object and light color
		glUniform3fv(objectColorLocation, 1, objectColor);
		glUniform3fv(lightColorLocation, 1, lightColor);
		
		// Transformation matrices
		mat4 lightModel, cubeModel, projection;
		glm_mat4_identity(cubeModel);
		glm_mat4_identity(projection);

		// Calculating the view matrix
		updateCam(&cam_light, window, deltaTime);
		mat4s view = glms_lookat(cam_light.pos, glms_vec3_add(cam_light.pos, cam_light.front), cam_light.up);
		
		// Transformation matrix calculations and sending them to the shaders
		glm_perspective(glm_rad(cam_light.fov), (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f, projection);

		glUniformMatrix4fv(cubeViewLocation, 1, GL_FALSE, *view.raw);
		glUniformMatrix4fv(cubeProjectionLocation, 1, GL_FALSE, projection[0]);

		// Set the cube position in the world
		glUniformMatrix4fv(cubeModelLocation, 1, GL_FALSE, cubeModel[0]);

		glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// render with the shader program and vertex array(s) 
		glUseProgram(lightShaderProgram);
		
		glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, *view.raw);
		glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, projection[0]);
		
		// Also set the light cube position in the world
		glm_mat4_identity(lightModel);
		glm_translate(lightModel, lightPos);
		glm_scale(lightModel, (vec3) { 0.2f, 0.2f, 0.2f });
		glUniformMatrix4fv(lightModelLocation, 1, GL_FALSE, lightModel[0]);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Ressource deallocation
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(colorShaderProgram);
	glDeleteProgram(lightShaderProgram);
	
	glfwTerminate();
	return 0;
}

void mouseCallbackLight(GLFWwindow* window, double xPos, double yPos)
{
	if(cam_light.firstMouse)
	{
		cam_light.lastMouseX = xPos;
		cam_light.lastMouseY = yPos;
		cam_light.firstMouse = false;
	}

	float offsetX = xPos - cam_light.lastMouseX;
	float offsetY = cam_light.lastMouseY - yPos;
	cam_light.lastMouseX = xPos;
	cam_light.lastMouseY = yPos;

	float sensitivity = 0.1f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	cam_light.yawAngle += offsetX;
	cam_light.pitchAngle += offsetY;

	if(cam_light.pitchAngle > 89.0f)
		cam_light.pitchAngle = 89.0f;
	if(cam_light.pitchAngle < -89.0f)
		cam_light.pitchAngle = -89.0f;
}

void scrollCallbackLight(GLFWwindow* window, double xoffset, double yoffset)
{
	cam_light.fov -= (float)yoffset;
    if (cam_light.fov < 1.0f)
        cam_light.fov = 1.0f;
    if (cam_light.fov > 65.0f)
        cam_light.fov = 65.0f; 
}