#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <Logger.h>
#include <chrono>

// function-like macro to check open gl errors, they had to be macros to get correct file name in logger
#define checkOpenGLErrors()															\
	GLenum err_macro;																\
	while ((err_macro = glGetError()) != GL_NO_ERROR){								\
		LOG_F(ERROR, "GL error: %s", gluErrorString(err_macro));					\
	}

long getTime();
std::chrono::time_point<std::chrono::steady_clock> getNanoTime();
double getNanoTimeDif(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> finish);
