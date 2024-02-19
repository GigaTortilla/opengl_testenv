#include <stdlib.h>
#include <stdio.h>
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