#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) 
{
	Shader vertexShader{ vertexShaderPath , "VERTEX" };
	Shader fragmentShader{ fragmentShaderPath, "FRAGMENT" };

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader.getID);
	glAttachShader(ID, fragmentShader.getID);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
}

void ShaderProgram::setUniformVariable(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ShaderProgram::setUniformVariable(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setUniformVariable(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setUniformVariable(const std::string& name, const glm::mat4& value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[INFO_LOG_SIZE];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, INFO_LOG_SIZE, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}
