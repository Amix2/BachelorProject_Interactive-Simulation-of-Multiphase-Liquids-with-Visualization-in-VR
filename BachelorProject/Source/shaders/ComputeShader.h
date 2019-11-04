#pragma once

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <Logger.h>
#include <shaders/ShaderCodeEditor.h>
#include <Utils.h>

class ComputeShader
{
	GLuint csProgram = -1;
	std::string m_shaderFileName;

	inline static std::unordered_map<std::string, GLint> m_UniformsMap;

public:
	ComputeShader(const std::string shaderFileName);
	ComputeShader() {}
	~ComputeShader();

	void setUniformVariable(const std::string& name, bool value) const;
	void setUniformVariable(const std::string& name, int value) const;
	void setUniformVariable(const std::string& name, float value) const;
	void setUniformVariable(const std::string& name, const glm::mat4& value) const;

	void runShader(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, bool block = false);
};

