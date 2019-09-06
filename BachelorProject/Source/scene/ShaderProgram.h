#pragma once
#include <glad/glad.h>
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
	static const unsigned int INFO_LOG_SIZE;
	unsigned int ID;

	void checkCompileErrors(unsigned int shader, std::string type);
};

const unsigned int ShaderProgram::INFO_LOG_SIZE = 1024;

