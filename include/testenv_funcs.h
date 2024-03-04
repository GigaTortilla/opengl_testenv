#ifndef TESTENV_FUNCS_H
#define TESTENV_FUNCS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/struct.h>

typedef struct {
    vec3s ambient;
    vec3s diffuse;
    vec3s specular;
    float shininess;
} Material;

void setWindowIcon(GLFWwindow* window, const char* path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void checkESC(GLFWwindow *window);
GLFWwindow* initWindow(int width, int height, const char* icon, const char* name);
unsigned int genBindTexRepeat(char* imagePath);
unsigned int genBindTexClamp2Edge(char* imagePath);

/// @brief Returns a vec3s with changing colors
/// @param t 	Time value for sine functions
vec3s testColorStrobe(float t);

/// @brief Updates the position of an object hovering around a specified point
/// @param[in] pos  	Position coordinates to hover around
/// @param[in] radius 	Distance from the specified position
/// @param[in] freq 	Rotational frequency 
/// @param[in] height 	Height above the specified point
/// @param[in] t 		Current time for sine function
/// @return         	The new position of the object as a vec3s
/// @author 			GigaTortilla
vec3s hoverAroundPoint(vec3s pos, float radius, float freq, float height, float t);

int f_blending();
int f_hello_triangle();
int f_practice_triangle();
int f_tex_tri();
int f_tri_insanity();
int f_coord_sys();
int f_cubes();
int f_lighting();
int f_materials();
int f_diffuseMap();

#endif