#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	Shader(std::string shaderPath, std::string type);
	~Shader() { glDeleteShader(ID); }

	unsigned int getID() { return ID; }

private:
	static const unsigned int INFO_LOG_SIZE;

	unsigned int ID;

	void checkCompileErrors(unsigned int shader, std::string type);
};

const unsigned int Shader::INFO_LOG_SIZE = 1024;
#endif