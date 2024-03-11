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
Camera cam_spot = {
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
	float cube[] = {
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
	unsigned int colorShaderProgram = buildShaderProgram("spotlight.vert", "spotlight.frag");
	unsigned int lightShaderProgram = buildShaderProgram("lightCube.vert", "lightMaterials.frag");
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBO, cubeVAO, lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	
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

	////////////////////////////////
	/// Shader uniform addresses ///
	////////////////////////////////

	// Colored cube vertex shader stage
	unsigned int cubeModelLocation = glGetUniformLocation(colorShaderProgram, "model");
	unsigned int cubeModelInvLocation = glGetUniformLocation(colorShaderProgram, "modelInv");
	unsigned int cubeViewLocation = glGetUniformLocation(colorShaderProgram, "view");
	unsigned int cubeProjectionLocation = glGetUniformLocation(colorShaderProgram, "projection");

	// Fragment shader material uniform
	unsigned int shininessMaterialLocation = glGetUniformLocation(colorShaderProgram, "material.shininess");
	
	// Fragment shader camera position uniform 
	unsigned int viewPosLocation = glGetUniformLocation(colorShaderProgram, "viewPos");

	// Directional light
	unsigned int dirLightDirLocation = glGetUniformLocation(colorShaderProgram, "direcionalLight.direction");
	unsigned int dirLightAmbLocation = glGetUniformLocation(colorShaderProgram, "direcionalLight.ambient");
	unsigned int dirLightDiffLocation = glGetUniformLocation(colorShaderProgram, "direcionalLight.diffuse");
	unsigned int dirLightSpecLocation = glGetUniformLocation(colorShaderProgram, "direcionalLight.specular");

	// Point light
	unsigned int ptLightPosLocation = glGetUniformLocation(colorShaderProgram, "pointLight.position");
	unsigned int ptLightConstLocation = glGetUniformLocation(colorShaderProgram, "pointLight.constant");
	unsigned int ptLightLinLocation = glGetUniformLocation(colorShaderProgram, "pointLight.linear");
	unsigned int ptLightQuadLocation = glGetUniformLocation(colorShaderProgram, "pointLight.quadratic");
	unsigned int ptLightAmbLocation = glGetUniformLocation(colorShaderProgram, "pointLight.ambient");
	unsigned int ptLightDiffLocation = glGetUniformLocation(colorShaderProgram, "pointLight.diffuse");
	unsigned int ptLightSpecLocation = glGetUniformLocation(colorShaderProgram, "pointLight.specular");

	// Spotlight
	unsigned int spotLightPosLocation = glGetUniformLocation(colorShaderProgram, "spotlight.position");
	unsigned int spotLightDirLocation = glGetUniformLocation(colorShaderProgram, "spotlight.direction");
	unsigned int spotLightCutOffLocation = glGetUniformLocation(colorShaderProgram, "spotlight.cutOff");
	unsigned int spotLightOutCutOffLocation = glGetUniformLocation(colorShaderProgram, "spotlight.outerCutOff");
	unsigned int spotLightConstLocation = glGetUniformLocation(colorShaderProgram, "spotlight.constant");
	unsigned int spotLightLinLocation = glGetUniformLocation(colorShaderProgram, "spotlight.linear");
	unsigned int spotLightQuadLocation = glGetUniformLocation(colorShaderProgram, "spotlight.quadratic");
	unsigned int spotLightAmbLocation = glGetUniformLocation(colorShaderProgram, "spotlight.ambient");
	unsigned int spotLightDiffLocation = glGetUniformLocation(colorShaderProgram, "spotlight.diffuse");
	unsigned int spotLightSpecLocation = glGetUniformLocation(colorShaderProgram, "spotlight.specular");

	// time-dependent fun stuff
	unsigned int timeLocation = glGetUniformLocation(colorShaderProgram, "time");

	//////////////////////////////////////////
	/// Light cube shader program uniforms ///
	//////////////////////////////////////////

	unsigned int lightModelLocation = glGetUniformLocation(lightShaderProgram, "model");
	unsigned int lightViewLocation = glGetUniformLocation(lightShaderProgram, "view");
	unsigned int lightProjectionLocation = glGetUniformLocation(lightShaderProgram, "projection");
	unsigned int lightColorLocation = glGetUniformLocation(lightShaderProgram, "lightColor");

	// lighting
	vec3s lightPos = {{ 1.0f, 0.1f, -6.0f }};
	vec3s lightColor = {{ 1.0f, 1.0f, 1.0f }};
	float lightCutOff = 0.95f;
	float lightOuterCutOff = lightCutOff - 0.02f;

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

		// Update light cone
		if (glfwGetKey(window, GLFW_KEY_R))
		{
			lightCutOff = (lightCutOff < 0.995f) ? lightCutOff + 0.05f * deltaTime : lightCutOff;
			lightOuterCutOff = lightCutOff - 0.02f;
		}
		if (glfwGetKey(window, GLFW_KEY_F))
		{
			lightCutOff = (lightCutOff > 0.9f) ? lightCutOff - 0.05f * deltaTime : lightCutOff;
			lightOuterCutOff = lightCutOff - 0.02f;
		}

		// // Update light color values
		// lightColor = testColorStrobe(timeValue);
		
		// clear screen first
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Update light cube position
		lightPos.z = 3.0f * sin(2.0f * timeValue) - 5.0f;

		// Calculating the view matrix
		updateCam(&cam_spot, window, deltaTime);
		mat4s view = glms_lookat(cam_spot.pos, glms_vec3_add(cam_spot.pos, cam_spot.front), cam_spot.up);
		
		// Projection matrix calculation
		mat4 projection;
		glm_mat4_identity(projection);
		glm_perspective(glm_rad(cam_spot.fov), (float)SCREEN_WIDTH_SXGA/SCREEN_HEIGHT_SXGA, 0.1f, 100.0f, projection);
		
		////////////////////////
		///// Colored cube /////
		////////////////////////
		
		// render with the shader program and vertex array(s) 
		glUseProgram(colorShaderProgram);

		// Set material properties for lighting
		glUniform1f(shininessMaterialLocation, coral.shininess * 128.0f);

		// Light parameters
		glUniform3fv(dirLightDirLocation, 1, (vec3) { -0.2f, -1.0f, -0.3f });
		glUniform3fv(dirLightAmbLocation, 1, glms_vec3_scale(lightColor, 0.1f).raw);
		glUniform3fv(dirLightDiffLocation, 1, glms_vec3_scale(lightColor, 0.5f).raw);
		glUniform3fv(dirLightSpecLocation, 1, lightColor.raw);

		glUniform3fv(ptLightPosLocation, 1, lightPos.raw);
		glUniform1f(ptLightConstLocation, 1.0f);
		glUniform1f(ptLightLinLocation, 0.027f);
		glUniform1f(ptLightQuadLocation, 0.0028f);
		glUniform3fv(ptLightAmbLocation, 1, glms_vec3_scale(lightColor, 0.1f).raw);
		glUniform3fv(ptLightDiffLocation, 1, glms_vec3_scale(lightColor, 0.5f).raw);
		glUniform3fv(ptLightSpecLocation, 1, lightColor.raw);

		glUniform3fv(spotLightPosLocation, 1, cam_spot.pos.raw);
		glUniform3fv(spotLightDirLocation, 1, cam_spot.front.raw);
		glUniform1f(spotLightCutOffLocation, lightCutOff);
		glUniform1f(spotLightOutCutOffLocation, lightOuterCutOff);
		glUniform1f(spotLightConstLocation, 1.0f);
		glUniform1f(spotLightLinLocation, 0.027f);
		glUniform1f(spotLightQuadLocation, 0.0028f);
		glUniform3fv(spotLightAmbLocation, 1, glms_vec3_scale(lightColor, 0.1f).raw);
		glUniform3fv(spotLightDiffLocation, 1, glms_vec3_scale(lightColor, 0.5f).raw);
		glUniform3fv(spotLightSpecLocation, 1, lightColor.raw);

		// extra time-dependent fun stuff
		glUniform1f(timeLocation, timeValue);

		// Set light color and position for the cubes shader stage

		// Send the camera position to the objects shader
		glUniform3fv(viewPosLocation, 1, cam_spot.pos.raw);
		
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
	if(cam_spot.firstMouse)
	{
		cam_spot.lastMouseX = xPos;
		cam_spot.lastMouseY = yPos;
		cam_spot.firstMouse = false;
	}

	float offsetX = xPos - cam_spot.lastMouseX;
	float offsetY = cam_spot.lastMouseY - yPos;
	cam_spot.lastMouseX = xPos;
	cam_spot.lastMouseY = yPos;

	float sensitivity = 0.1f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	cam_spot.yawAngle += offsetX;
	cam_spot.pitchAngle += offsetY;

	if(cam_spot.pitchAngle > 89.0f)
		cam_spot.pitchAngle = 89.0f;
	if(cam_spot.pitchAngle < -89.0f)
		cam_spot.pitchAngle = -89.0f;
}
#pragma GCC diagnostic pop

// Callback function has a predetermined structure, thus the unused parameter warning is supressed
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// Callback function to scrolling zoom
void scrollCallbackSpot(GLFWwindow* window, double xoffset, double yoffset)
{
	cam_spot.fov -= (float)yoffset;
    if (cam_spot.fov < 1.0f)
        cam_spot.fov = 1.0f;
    if (cam_spot.fov > 65.0f)
        cam_spot.fov = 65.0f; 
}
#pragma GCC diagnostic pop