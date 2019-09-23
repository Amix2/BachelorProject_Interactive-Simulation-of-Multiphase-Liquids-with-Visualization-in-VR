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
	if (type == "VERTEX")
		ID = glCreateShader(GL_VERTEX_SHADER);
	else if (type == "FRAGMENT")
		ID = glCreateShader(GL_FRAGMENT_SHADER);
	else
		std::cout << "ERROR::INTERNAL bad shader type: " << type << std::endl;
	glShaderSource(ID, 1, &shaderCodeC, NULL);
	glCompileShader(ID);
	checkCompileErrors(type);
}

void Shader::checkCompileErrors(std::string type)
{
	int success;
	char infoLog[INFO_LOG_SIZE];

	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(ID, INFO_LOG_SIZE, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
	else {
		std::cout << "SUCCESS successful compilation of shader no. " << ID << ", of type: " << type << std::endl;
	}
}
