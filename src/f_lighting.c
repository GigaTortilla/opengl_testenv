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
	.pos = {{ -1.5f, 0.0f, 5.0f }},
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
	// Vertices array serves as data for displaying cubes. 
	float vertices[] = {
	// Position			 // Normal vector
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

	// Stores the time at the previous frame to calculate time differences between frames
	float lastFrame = 0.0f;

	GLFWwindow *window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "citrus.png", "OpenGLTestEnvironment");

	// hide and capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Bind callback functions
	glfwSetCursorPosCallback(window, mouseCallbackLight);
	glfwSetScrollCallback(window, scrollCallbackLight);

	// Build the shader programs
	// Since both programs use the same functions in the vertex shader stage, using the same vertex shader file is a good option
	unsigned int colorShaderProgram = buildShaderProgram("colorShader.vert", "colorShader.frag");
	unsigned int lightShaderProgram = buildShaderProgram("lightCube.vert", "lightCube.frag");
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBO, cubeVAO, lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(cubeVAO);
	// Position attribute at attribute position 0 in the vertex shader layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Normal vector attribute at attribute position 1 in the vertex shader layout
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Repeat this process for the lightVAO 
	// We only need to bind the VBO again to set glVertexAttribPointer
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Position attribute at attribute position 0 in the vertex shader layout
	// Updated only the lamps position attribute stride since we don't want to shade the light cube differently 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Finding the location of the shader uniforms

	unsigned int cubeModelLocation = glGetUniformLocation(colorShaderProgram, "model");
	unsigned int cubeModelInvLocation = glGetUniformLocation(colorShaderProgram, "modelInv");
	unsigned int cubeViewLocation = glGetUniformLocation(colorShaderProgram, "view");
	unsigned int cubeProjectionLocation = glGetUniformLocation(colorShaderProgram, "projection");

	unsigned int lightModelLocation = glGetUniformLocation(lightShaderProgram, "model");
	unsigned int lightViewLocation = glGetUniformLocation(lightShaderProgram, "view");
	unsigned int lightProjectionLocation = glGetUniformLocation(lightShaderProgram, "projection");

	unsigned int objectColorLocation = glGetUniformLocation(colorShaderProgram, "objectColor");
	unsigned int lightColorLocation = glGetUniformLocation(colorShaderProgram, "lightColor");
	unsigned int lightPosLocation = glGetUniformLocation(colorShaderProgram, "lightPos");
	unsigned int viewPosLocation = glGetUniformLocation(colorShaderProgram, "viewPos");
	
	// lighting
	vec3s cubePos = {{ 0.0f, -1.0f, 0.0f }};
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
		
		// Update light cube position
		vec3 lightPos = { cubePos.x + 2.5f * sin(timeValue), cubePos.y + 1.0f, cubePos.z + 2.5f * cos(timeValue) };

		////////////////////////
		///// Colored cube /////
		////////////////////////
		
		// render with the shader program and vertex array(s) 
		glUseProgram(colorShaderProgram);

		// Set object and light color
		glUniform3fv(objectColorLocation, 1, objectColor);
		glUniform3fv(lightColorLocation, 1, lightColor);
		// Send the light and camera position to the objects shader
		glUniform3fv(lightPosLocation, 1, lightPos);
		glUniform3fv(viewPosLocation, 1, cam_light.pos.raw);
		
		// Transformation matrices
		mat4 projection;
		glm_mat4_identity(projection);
		mat4s cubeModel = glms_mat4_identity();
		cubeModel = glms_translate(cubeModel, cubePos);

		// Calculating the view matrix
		updateCam(&cam_light, window, deltaTime);
		mat4s view = glms_lookat(cam_light.pos, glms_vec3_add(cam_light.pos, cam_light.front), cam_light.up);
		
		// Projection matrix calculation
		glm_perspective(glm_rad(cam_light.fov), (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f, projection);

		// Sending the view and transformation matrix to the shader program
		glUniformMatrix4fv(cubeViewLocation, 1, GL_FALSE, *view.raw);
		glUniformMatrix4fv(cubeProjectionLocation, 1, GL_FALSE, projection[0]);

		mat4s cubeModelInv = glms_mat4_inv(cubeModel);
		// Set the cube position in the world
		glUniformMatrix4fv(cubeModelLocation, 1, GL_FALSE, *cubeModel.raw);
		glUniformMatrix4fv(cubeModelInvLocation, 1, GL_FALSE, *cubeModelInv.raw);

		glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

		//////////////////////
		///// Light cube /////
		//////////////////////
		
		// render with the shader program and vertex array(s) 
		glUseProgram(lightShaderProgram);
		
		glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, *view.raw);
		glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, projection[0]);
		
		// Also set the light cube position in the world
		mat4 lightModel;
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

// Callback function has a predetermined structure, thus the unused parameter warning is supressed
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// Callback function to enable camera control using the mouse cursor
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
#pragma GCC diagnostic pop

// Callback function has a predetermined structure, thus the unused parameter warning is supressed
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// Callback function to scrolling zoom
void scrollCallbackLight(GLFWwindow* window, double xoffset, double yoffset)
{
	cam_light.fov -= (float)yoffset;
    if (cam_light.fov < 1.0f)
        cam_light.fov = 1.0f;
    if (cam_light.fov > 65.0f)
        cam_light.fov = 65.0f; 
}
#pragma GCC diagnostic pop