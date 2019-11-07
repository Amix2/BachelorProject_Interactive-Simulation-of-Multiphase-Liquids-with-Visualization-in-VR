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
		LOG_F(ERROR, "ERROR::SHADER::%s::FILE_NOT_SUCCESFULLY_READ", type.c_str());
	}

	const char* shaderCodeC = shaderCode.c_str();
	if (type == "VERTEX")
		ID = glCreateShader(GL_VERTEX_SHADER);
	else if (type == "FRAGMENT")
		ID = glCreateShader(GL_FRAGMENT_SHADER);
	else if (type == "GEOMETRY")
		ID = glCreateShader(GL_GEOMETRY_SHADER);
	else
		LOG_F(ERROR, "ERROR::INTERNAL bad shader type: %s", type);
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
		LOG_F(ERROR, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- ", type.c_str(), infoLog);
	}
	else {
		LOG_F(INFO, "SUCCESS successful compilation of shader no. %d, of type: %s", ID, type.c_str());
	}
}
