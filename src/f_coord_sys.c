#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <myshaders.h>
#include <testenv_funcs.h>

// WARNING: #define STB_IMAGE_IMPLEMENTATION in main.c
#include <stb_image.h>

int f_coord_sys()
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
	if (!window)
	{
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Somehow setting the viewport here after window creation crashes the application
	// glViewport(0, 0, 800, 600);		// Sets the viewport to the lower left corner with size 800x600
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	setWindowIcon(window, "textures/citrus.png");
	
	// Initialize and load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return -1;
	}
	
	////////////////////////////////////////////
	/// Build and compile the shader program ///
	////////////////////////////////////////////
	unsigned int shaderProgram = buildShaderProgram("transformShader.vert", "transformShader.frag");
	
	// Example data 
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
	
	// Set up Vertex Buffer Object and Vertex Array Object
	unsigned int VBOs[2], VAOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Position attribute at attribute position 0 in the vertex shader layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Color attribute at attribute position 1 in the vertex shader layout
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture position attribute at attribute position 2 in the vertex shader layout
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Repeat this step for the second square
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	// unbind only when necessary
	// since VAOs require a call to glBindVertexArray to be modified anyways
	glBindVertexArray(0);
	
	int width, height, nrChannels;
	unsigned int texture1, texture2;
	stbi_set_flip_vertically_on_load(GL_TRUE);
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);
	
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		printf("Texture could not be loaded!");
	}
	stbi_image_free(data);
	
	glBindTexture(GL_TEXTURE_2D, texture2);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		printf("Texture could not be loaded!");
	}
	stbi_image_free(data);
	
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

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
		
		// Bind the textures on the corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// render with the shader program and vertex array(s) 
		glUseProgram(shaderProgram);

		float timeValue = glfwGetTime();
		float scaleValue = sin(1.8f * timeValue) * 0.2f + 0.8f;
		float blendValue = (sin(2.5f * timeValue) + 1.0f) * 0.35f;
		float offsetValue = sin(2.0f * timeValue) * 0.3f;
		
		mat4 model, view, projection;
		glm_mat4_identity(model);
		glm_mat4_identity(view);
		glm_mat4_identity(projection);
		
		// Transformation matrix calculations
		glm_translate(model, (vec3) { 0.5f + offsetValue, 0.5f, 0.0f });
		glm_rotate(model, glm_rad(-55.0f), (vec3) { 1.0f, 0.0f, 0.0f });

		glm_translate(view, (vec3) { 0.0f, 0.0f, -3.0f });
		glm_perspective(glm_rad(45.0f), 800.0f/600.0f, 0.1f, 100.0f, projection);

		// sending the transformation matrices to the vertex shader
		unsigned int modelLocation = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
		unsigned int blendLocation = glGetUniformLocation(shaderProgram, "blend");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model[0]);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view[0]);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection[0]);
		glUniform1f(blendLocation, blendValue);
		
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		///////////////////////////////////////////////////////////////////////////////////////////
		
		glm_mat4_identity(model);
		glm_translate(model, (vec3) { -0.3f, -0.4f, 0.0f });
		glm_rotate(model, glm_rad(-55.0f), (vec3) { 1.0f, 0.0f, 0.0f });
		glm_rotate_z(model, timeValue, model);
		glm_scale(model, (vec3) { scaleValue, scaleValue, 0.0f });

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model[0]);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view[0]);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection[0]);
		glUniform1f(blendLocation, blendValue);

		glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Ressource deallocation
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
    glDeleteBuffers(2, EBOs);
	glDeleteProgram(shaderProgram);
	
	glfwTerminate();
	return 0;
}
