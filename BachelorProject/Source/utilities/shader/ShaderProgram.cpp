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

ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath)
{
	Shader vertexShader{ vertexShaderPath , "VERTEX" };
	Shader geometryShader{ geometryShaderPath, "GEOMETRY" };
	Shader fragmentShader{ fragmentShaderPath, "FRAGMENT" };


	ID = glCreateProgram();
	glAttachShader(ID, vertexShader.getID());
	glAttachShader(ID, geometryShader.getID());
	glAttachShader(ID, fragmentShader.getID());
	glLinkProgram(ID);
	checkLinkingErrors();
}

void ShaderProgram::setUniformVariable(const std::string& name, bool value) const
{
	glUniform1i(getUniformLocation(name), (int)value);
}

void ShaderProgram::setUniformVariable(const std::string& name, int value) const
{
	glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setUniformVariable(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setUniformVariable(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setUniformVariable(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(getUniformLocation(name), 1, &value[0]);
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
		LOG_F(INFO, "SUCCESS successful linking of program no. %d ", ID);
	}
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const
{
	GLint uniformIdx;
	if (m_UniformsMap.find(name) != m_UniformsMap.end()) {
		return m_UniformsMap[name];

	}
	uniformIdx = glGetUniformLocation(ID, name.c_str());
	m_UniformsMap[name] = uniformIdx;
	return uniformIdx;
}
