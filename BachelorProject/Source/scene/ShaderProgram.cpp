#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) 
{
	Shader vertexShader{ vertexShaderPath , "VERTEX" };
	Shader fragmentShader{ fragmentShaderPath, "FRAGMENT" };

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader.getID());
	glAttachShader(ID, fragmentShader.getID());
	glLinkProgram(ID);
	checkLinkingErrors();
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
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::checkLinkingErrors()
{
	int success;
	char infoLog[INFO_LOG_SIZE];

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetProgramInfoLog(ID, INFO_LOG_SIZE, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
	else {
		std::cout << "SUCCESS successful linking of program no. " << ID << std::endl;
	}
}
