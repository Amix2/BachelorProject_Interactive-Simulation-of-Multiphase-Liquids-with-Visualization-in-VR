#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <unordered_map>

#include "Shader.h"


class ShaderProgram
{
public:
	ShaderProgram() {}
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	ShaderProgram(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath);

	unsigned int getID() { return ID; }

	void use() const { glUseProgram(ID); }
	void setUniformVariable(const std::string& name, bool value) const;
	void setUniformVariable(const std::string& name, int value) const;
	void setUniformVariable(const std::string& name, float value) const;
	void setUniformVariable(const std::string& name, const glm::mat4& value) const;
	void setUniformVariable(const std::string& name, const glm::vec4& value) const;
private:
	inline static const unsigned int INFO_LOG_SIZE{ 1024 };
	unsigned int ID;

	void checkLinkingErrors();

	mutable std::unordered_map<std::string, GLint> m_UniformsMap;
	GLint getUniformLocation(const std::string& name) const;
};

