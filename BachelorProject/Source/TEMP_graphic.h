#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class TEMP_graphic
{
public:
	static inline int shaderProgram = -1;
	static void initGraphic(GLFWwindow* window);
	static void showFrame(GLFWwindow* window);
};
