#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <Logger.h>
#include <chrono>
#include <glm/glm.hpp>


// function-like macro to check open gl errors, they had to be macros to get correct file name in logger
#define checkOpenGLErrors()															\
	GLenum err_macro;																\
	while ((err_macro = glGetError()) != GL_NO_ERROR){								\
		LOG_F(ERROR, "GL error: %s", gluErrorString(err_macro));					\
	}

namespace Utils {

	long getTime();
	std::chrono::time_point<std::chrono::steady_clock> getNanoTime();
	double getNanoTimeDif(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> finish);

	glm::vec3 getUp(const glm::mat4 matrix);
	glm::vec3 getRight(const glm::mat4 matrix);
	glm::vec3 getForward(const glm::mat4 matrix);
	glm::vec3 getPosition(const glm::mat4 matrix);
	void setUp(glm::mat4 *matrix, const glm::vec3 &vector);
	void setRight(glm::mat4 *matrix, const glm::vec3& vector);
	void setForward(glm::mat4 *matrix, const glm::vec3& vector);
	void setPosition(glm::mat4 *matrix, const glm::vec3& vector);
	//std::string glm_to_string(glm::mat4 mat) { return glm::to_string(mat); }
	//std::string glm_to_string(glm::vec3 vec) { return glm::to_string(vec); }
	//std::string glm_to_string(glm::vec4 vec) { return glm::to_string(vec); }

	glm::mat4 getRotationMatrix(glm::vec3 axis, float angle);
}