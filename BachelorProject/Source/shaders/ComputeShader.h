#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

class ComputeShader
{
	GLuint csProgram;
	mutable std::unordered_map<std::string, GLuint> bufferMap;

public:
	ComputeShader(const std::string shaderFileName);
	~ComputeShader();

	void runShader(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, bool block = false);

	void bindSSBO(std::string name, GLsizeiptr size, const void *data, GLuint bindingPointIndex) const;
	void* getSSBO(std::string name);
};

