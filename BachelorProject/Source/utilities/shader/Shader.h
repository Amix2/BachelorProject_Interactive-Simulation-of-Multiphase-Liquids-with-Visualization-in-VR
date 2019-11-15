#ifndef SHADER_H
#define SHADER_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Logger.h>

class Shader
{
public:
	Shader(std::string shaderPath, std::string type);
	~Shader() { glDeleteShader(ID); }

	unsigned int getID() { return ID; }

private:
	inline static const unsigned int INFO_LOG_SIZE{ 1024 };

	unsigned int ID;

	void checkCompileErrors(std::string type);
};
#endif