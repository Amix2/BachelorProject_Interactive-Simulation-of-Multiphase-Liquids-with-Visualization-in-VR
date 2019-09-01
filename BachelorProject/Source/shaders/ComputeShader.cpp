#include "ComputeShader.h"

ComputeShader::ComputeShader(const std::string shaderFileName)
{
	GLuint csCreatorID = glCreateShader(GL_COMPUTE_SHADER);

	std::ifstream csInputFileStream(shaderFileName);
	std::string computeShaderCode((std::istreambuf_iterator<char>(csInputFileStream)), std::istreambuf_iterator<char>());

	ShaderCodeEditor::insertVariables(computeShaderCode);

	const char* c_source = computeShaderCode.c_str();

	glShaderSource(csCreatorID, 1, &c_source, NULL);
	glCompileShader(csCreatorID);

	GLint Result = GL_FALSE;
	int InfoLogLength = 1024;
	char shaderErrorMessage[1024] = { 0 };

	glGetShaderiv(csCreatorID, GL_COMPILE_STATUS, &Result);

	if(Result == GL_FALSE)
	{
		int length;
		glGetShaderiv(csCreatorID, GL_INFO_LOG_LENGTH, &length);

		GLchar* strInfoLog = new GLchar[(size_t)length + 1];
		glGetShaderInfoLog(csCreatorID, length, &length, strInfoLog);

		fprintf(stderr, "Compilation error in shader: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	this->csProgram = glCreateProgram();			
	glAttachShader(this->csProgram, csCreatorID);
	glLinkProgram(this->csProgram);
	glDeleteShader(csCreatorID);
}


ComputeShader::~ComputeShader()
{
}

void ComputeShader::runShader(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, bool block)
{
	glUseProgram(this->csProgram);
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);  
	//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	if (block) {
		glFinish();
	}
	checkOpenGLErrors();
}
