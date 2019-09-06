#include "Shader.h"

Shader::Shader(std::string shaderPath, std::string type)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	try
	{
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		shaderFile.open(shaderPath);
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* shaderCodeC = shaderCode.c_str();
	ID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ID, 1, &shaderCodeC, NULL);
	glCompileShader(ID);
	checkCompileErrors(ID, type);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[INFO_LOG_SIZE];

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, INFO_LOG_SIZE, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}
