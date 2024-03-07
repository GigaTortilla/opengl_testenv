#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <myshaders.h>
#include <camera/cam.h>
#include <testenv_funcs.h>

// WARNING: #define STB_IMAGE_IMPLEMENTATION in main.c
#include <stb_image.h>

// Global camera struct initialization
Camera cam_more = {
	.pos = {{ -1.5f, 0.0f, 5.0f }},
	.front = {{ 0.0f, 0.0f, -1.0f }},
	.right = {{ 1.0f, 0.0f, 0.0f }},
	.up = {{ 0.0f, 1.0f, 0.0f }},

	.fov = 45.0f,
	.speed = 0.05f,

	.firstMouse = true,
	.lastMouseX = SCREEN_WIDTH_SXGA / 2.0f,
	.lastMouseY = SCREEN_HEIGHT_SXGA / 2.0f,

	.yawAngle = -90.0f,
	.pitchAngle = 0.0f
};

void mouseCallbackSpot(GLFWwindow* window, double xPos, double yPos);
void scrollCallbackSpot(GLFWwindow* window, double xoffset, double yoffset);

int f_spotlight()
{
	// Vertices array serves as data for displaying cubes. 
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	vec3s cubePositions[] = {
		{{  0.0f,  0.0f,  0.0f  }},
		{{  2.0f,  5.0f, -15.0f }},
		{{ -1.5f, -2.2f, -2.5f  }},
		{{ -3.8f, -2.0f, -12.3f }},
		{{  2.4f, -0.4f, -3.5f  }},
		{{ -1.7f,  3.0f, -7.5f  }},
		{{  1.3f, -2.0f, -2.5f  }},
		{{  1.5f,  2.0f, -2.5f  }},
		{{  1.5f,  0.2f, -1.5f  }},
		{{ -1.3f,  1.0f, -1.5f  }}
	};
	Material coral = {
		.ambient = {{ 1.0f, 0.5f, 0.31f }},
		.diffuse = {{ 1.0f, 0.5f, 0.31f }},
		.specular = {{ 0.5f, 0.5f, 0.5f }},
		.shininess = 0.5f
	};
	Material ruby = {
		.ambient = {{ 0.1745f, 0.01175f, 0.01175f }},
		.diffuse = {{ 0.61424f, 0.04136f, 0.04136f }},
		.specular = {{ 0.727811f, 0.626959f, 0.626959f }},
		.shininess = 0.6f
	};

	// Stores the time at the previous frame to calculate time differences between frames
	float lastFrame = 0.0f;

	GLFWwindow *window = initWindow(SCREEN_WIDTH_SXGA, SCREEN_HEIGHT_SXGA, "citrus.png", "OpenGLTestEnvironment");

	// hide and capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Bind callback functions
	glfwSetCursorPosCallback(window, mouseCallbackSpot);
	glfwSetScrollCallback(window, scrollCallbackSpot);

	// Build the shader programs
	unsigned int colorShaderProgram = buildShaderProgram("moreLight.vert", "moreLight.frag");
	unsigned int lightShaderProgram = buildShaderProgram("lightCube.vert", "lightMaterials.frag");
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBO, cubeVAO, lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(cubeVAO);
	// Position attribute at attribute position 0 in the vertex shader layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Normal vector attribute at attribute position 1 in the vertex shader layout
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture coordinates attribute at attribute position 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Repeat this process for the lightVAO 
	// We only need to bind the VBO again to set glVertexAttribPointer
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Position attribute at attribute position 0 in the vertex shader layout
	// Updated only the lamps position attribute stride since we don't want to shade the light cube differently 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	////////////////////
	///// Textures /////
	////////////////////
	unsigned int textureBox = genBindTexRepeat("textures/container2.png");
	unsigned int textureBoxSpecMap = genBindTexRepeat("textures/lighting_maps_specular_color.png");
	unsigned int texBoxEmissionMap = genBindTexRepeat("textures/container2_emission.png");
	glUseProgram(colorShaderProgram);
	glUniform1i(glGetUniformLocation(colorShaderProgram, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(colorShaderProgram, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(colorShaderProgram, "material.emission"), 2);

	// Finding the location of the shader uniforms

	// Colored cube shader program
	unsigned int cubeModelLocation = glGetUniformLocation(colorShaderProgram, "model");
	unsigned int cubeModelInvLocation = glGetUniformLocation(colorShaderProgram, "modelInv");
	unsigned int cubeViewLocation = glGetUniformLocation(colorShaderProgram, "view");
	unsigned int cubeProjectionLocation = glGetUniformLocation(colorShaderProgram, "projection");

	// Lighting uniforms in shader program
	unsigned int viewPosLocation = glGetUniformLocation(colorShaderProgram, "viewPos");
	unsigned int shininessMaterialLocation = glGetUniformLocation(colorShaderProgram, "material.shininess");
	unsigned int ambLightLocation = glGetUniformLocation(colorShaderProgram, "light.ambient");
	unsigned int diffLightLocation = glGetUniformLocation(colorShaderProgram, "light.diffuse");
	unsigned int specLightLocation = glGetUniformLocation(colorShaderProgram, "light.specular");
	unsigned int lightPosLocation = glGetUniformLocation(colorShaderProgram, "light.position");

	// Distance parameters
	unsigned int lightConstantLocation = glGetUniformLocation(colorShaderProgram, "light.constant");
	unsigned int lightLinearLocation = glGetUniformLocation(colorShaderProgram, "light.linear");
	unsigned int lightQuadraticLocation = glGetUniformLocation(colorShaderProgram, "light.quadratic");

	// time-dependent fun stuff
	unsigned int timeLocation = glGetUniformLocation(colorShaderProgram, "time");
	
	// Light cube shader program
	unsigned int lightModelLocation = glGetUniformLocation(lightShaderProgram, "model");
	unsigned int lightViewLocation = glGetUniformLocation(lightShaderProgram, "view");
	unsigned int lightProjectionLocation = glGetUniformLocation(lightShaderProgram, "projection");
	unsigned int lightColorLocation = glGetUniformLocation(lightShaderProgram, "lightColor");

	// lighting
	vec3s lightPos = {{ 1.0f, 0.1f, -6.0f }};
	vec3s lightColor = {{ 1.0f, 1.0f, 1.0f }};

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

		// // Update light color values
		// lightColor = testColorStrobe(timeValue);
		
		// clear screen first
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Update light cube position
		lightPos.z = 3.0f * sin(2.0f * timeValue) - 5.0f;

		// Calculating the view matrix
		updateCam(&cam_more, window, deltaTime);
		mat4s view = glms_lookat(cam_more.pos, glms_vec3_add(cam_more.pos, cam_more.front), cam_more.up);
		
		// Projection matrix calculation
		mat4 projection;
		glm_mat4_identity(projection);
		glm_perspective(glm_rad(cam_more.fov), (float)SCREEN_WIDTH_SXGA/SCREEN_HEIGHT_SXGA, 0.1f, 100.0f, projection);
		
		////////////////////////
		///// Colored cube /////
		////////////////////////
		
		// render with the shader program and vertex array(s) 
		glUseProgram(colorShaderProgram);

		// Set material properties for lighting
		glUniform1f(shininessMaterialLocation, coral.shininess * 128.0f);

		// Light attenuation parameters
		glUniform1f(lightConstantLocation, 1.0f);
		glUniform1f(lightLinearLocation, 0.07f);
		glUniform1f(lightQuadraticLocation, 0.017f);

		// extra time-dependent fun stuff
		glUniform1f(timeLocation, timeValue);

		// Set light color and position for the cubes shader stage
		glUniform3fv(ambLightLocation, 1, glms_vec3_scale(lightColor, 0.1f).raw);
		glUniform3fv(diffLightLocation, 1, glms_vec3_scale(lightColor, 0.5f).raw);
		glUniform3fv(specLightLocation, 1, lightColor.raw);
		glUniform3fv(lightPosLocation, 1, lightPos.raw);

		// Send the camera position to the objects shader
		glUniform3fv(viewPosLocation, 1, cam_more.pos.raw);
		
		// Sending the view and transformation matrices to the shader program
		glUniformMatrix4fv(cubeViewLocation, 1, GL_FALSE, *view.raw);
		glUniformMatrix4fv(cubeProjectionLocation, 1, GL_FALSE, projection[0]);

		// Bind the textures on the corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBox);				/// Box texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureBoxSpecMap);		/// Specular map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texBoxEmissionMap);		/// Emission map

		for(unsigned int i = 0; i < 10; i++)
		{
			mat4s cubeModel = glms_mat4_identity();
			cubeModel = glms_translate(cubeModel, cubePositions[i]);
			cubeModel = glms_rotate(cubeModel, glm_rad(20.0f * i), (vec3s) {{ 1.0f, 0.3f, 0.5f }});
			mat4s cubeModelInv = glms_mat4_inv(cubeModel);
			// Set the cube position in the world.
			// Sending the transposed inverse of the model matrix to the vertex shader 
			// for transforming the normal vectors to world space.
			glUniformMatrix4fv(cubeModelLocation, 1, GL_FALSE, *cubeModel.raw);
			glUniformMatrix4fv(cubeModelInvLocation, 1, GL_TRUE, *cubeModelInv.raw);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//////////////////////
		///// Light cube /////
		//////////////////////
		
		// render with the shader program and vertex array(s) 
		glUseProgram(lightShaderProgram);

		// Sending the light sources color to the shader to comply with cube lighting 
		glUniform3fv(lightColorLocation, 1, lightColor.raw);
		
		// Send view and projection matrices to the vertex shader stage
		// using the matrices calculated for the colored cube
		glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, *view.raw);
		glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, projection[0]);
		
		// Also set the light cube position in the world
		mat4s lightModel = glms_mat4_identity();
		lightModel = glms_translate(lightModel, lightPos);
		lightModel = glms_scale(lightModel, (vec3s) {{ 0.2f, 0.2f, 0.2f }}); // uniform scaling could also be done by glms_vec3_scale()
		glUniformMatrix4fv(lightModelLocation, 1, GL_FALSE, *lightModel.raw);

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
void mouseCallbackSpot(GLFWwindow* window, double xPos, double yPos)
{
	if(cam_more.firstMouse)
	{
		cam_more.lastMouseX = xPos;
		cam_more.lastMouseY = yPos;
		cam_more.firstMouse = false;
	}

	float offsetX = xPos - cam_more.lastMouseX;
	float offsetY = cam_more.lastMouseY - yPos;
	cam_more.lastMouseX = xPos;
	cam_more.lastMouseY = yPos;

	float sensitivity = 0.1f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	cam_more.yawAngle += offsetX;
	cam_more.pitchAngle += offsetY;

	if(cam_more.pitchAngle > 89.0f)
		cam_more.pitchAngle = 89.0f;
	if(cam_more.pitchAngle < -89.0f)
		cam_more.pitchAngle = -89.0f;
}
#pragma GCC diagnostic pop

// Callback function has a predetermined structure, thus the unused parameter warning is supressed
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// Callback function to scrolling zoom
void scrollCallbackSpot(GLFWwindow* window, double xoffset, double yoffset)
{
	cam_more.fov -= (float)yoffset;
    if (cam_more.fov < 1.0f)
        cam_more.fov = 1.0f;
    if (cam_more.fov > 65.0f)
        cam_more.fov = 65.0f; 
}
#pragma GCC diagnostic pop