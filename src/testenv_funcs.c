#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Sets the window icon to the image in the specified path
// Prefer to use .png files
void setWindowIcon(GLFWwindow* window, const char* path)
{
	GLFWimage icons[1];
	int iconWidth, iconHeight, iconNrChannels;
	icons[0].pixels = stbi_load(path, &iconWidth, &iconHeight, &iconNrChannels, 0);
	if(icons[0].pixels)
	{
		icons[0].width = iconWidth;
		icons[0].height = iconHeight;
		glfwSetWindowIcon(window, 1, icons);
	}
	else 
	{
		printf("Icon could not be loaded!");
	}
	stbi_image_free(icons[0].pixels);
}

// Processes all inputs for the render loop
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Whenever the window size changed this callback function executes
void checkESC(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* initWindow(int width, int height, const char* icon, const char* name)
{
	char *icoPath = calloc(256, sizeof(char));
	strcat(icoPath, "textures/");
	strcat(icoPath, icon);

	// Initialize and configure GLFW
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// Tells GLFW that we're using OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// required under Mac OS X
#endif
	
	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (!window)
	{
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	// Somehow setting the viewport here after window creation crashes the application
	// glViewport(0, 0, 800, 600);		// Sets the viewport to the lower left corner with size 800x600
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	setWindowIcon(window, icoPath);
	
	// Initialize and load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return NULL;
	}
	
	glEnable(GL_DEPTH_TEST);
	return window;
}

unsigned int genBindTexRepeat(char* imagePath)
{
	unsigned int texture;
	int width, height, nrChannels;
	glGenTextures(1, &texture);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Texture parameter settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if(data)
	{
		if(nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if(nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		printf("Texture could not be loaded!");
	}
	stbi_image_free(data);
	return texture;
}