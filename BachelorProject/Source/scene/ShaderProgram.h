#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>
#include <iostream>

#include "Shader.h"


class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);

	unsigned int getID() { return ID; }

	void use() const { glUseProgram(ID); }
	void setUniformVariable(const std::string& name, bool value) const;
	void setUniformVariable(const std::string& name, int value) const;
	void setUniformVariable(const std::string& name, float value) const;
	void setUniformVariable(const std::string& name, const glm::mat4& value) const;
private:
	inline static const unsigned int INFO_LOG_SIZE{ 1024 };
	unsigned int ID;

	void checkLinkingErrors();
};

