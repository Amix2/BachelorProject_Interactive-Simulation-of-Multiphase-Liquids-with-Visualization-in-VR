#include "ComputeShader.h"

ComputeShader::ComputeShader(const std::string shaderFileName)
{
	auto ntStart = getNanoTime();
	GLuint csCreatorID = glCreateShader(GL_COMPUTE_SHADER);

	std::ifstream csInputFileStream(shaderFileName);
	std::string computeShaderCode((std::istreambuf_iterator<char>(csInputFileStream)), std::istreambuf_iterator<char>());

	ShaderCodeEditor::insertVariables(computeShaderCode);

	const char* c_source = computeShaderCode.c_str();
	//LOG_F(ERROR, "\n%s", c_source);

	glShaderSource(csCreatorID, 1, &c_source, NULL);
	glCompileShader(csCreatorID);

	GLint Result = GL_FALSE;
	int InfoLogLength = 1024;
	char shaderErrorMessage[1024] = { 0 };

	glGetShaderiv(csCreatorID, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE)
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

	this->m_shaderFileName = shaderFileName;

	auto ntEnd = getNanoTime();
	LOG_F(WARNING, "Shader %s compile successful, time: %f", shaderFileName.c_str(), getNanoTimeDif(ntStart, ntEnd));
}


ComputeShader::~ComputeShader()
{
}

void ComputeShader::setUniformVariable(const std::string& name, bool value) const
{
	if (m_UniformsMap.find(name) != m_UniformsMap.end()) {
		glUseProgram(this->csProgram);
		glUniform1i(m_UniformsMap[name], (int)value);

	}
	else {
		GLint uniformIdx = glGetUniformLocation(csProgram, name.c_str());
		m_UniformsMap.insert({ name, uniformIdx });
		glUseProgram(this->csProgram);
		glUniform1i(uniformIdx, (int)value);
	}
	checkOpenGLErrors();
}

void ComputeShader::setUniformVariable(const std::string& name, int value) const
{

	if (m_UniformsMap.find(name) != m_UniformsMap.end()) {
		glUseProgram(this->csProgram);
		glUniform1i(m_UniformsMap[name], value);

	}
	else {
		GLint uniformIdx = glGetUniformLocation(csProgram, name.c_str());
		m_UniformsMap.insert({ name, uniformIdx });
		glUseProgram(this->csProgram);
		glUniform1i(uniformIdx, value);
	}
	checkOpenGLErrors();
}

void ComputeShader::setUniformVariable(const std::string& name, float value) const
{
	if (m_UniformsMap.find(name) != m_UniformsMap.end()) {
		glUseProgram(this->csProgram);
		glUniform1f(m_UniformsMap[name], value);

	}
	else {
		GLint uniformIdx = glGetUniformLocation(csProgram, name.c_str());
		m_UniformsMap.insert({ name, uniformIdx });
		glUseProgram(this->csProgram);
		glUniform1f(uniformIdx, value);
	}
	checkOpenGLErrors();
}

void ComputeShader::setUniformVariable(const std::string& name, const glm::mat4& value) const
{
	if (m_UniformsMap.find(name) != m_UniformsMap.end()) {
		glUseProgram(this->csProgram);
		glUniformMatrix4fv(m_UniformsMap[name], 1, GL_FALSE, & value[0][0]);

	}
	else {
		GLint uniformIdx = glGetUniformLocation(csProgram, name.c_str());
		m_UniformsMap.insert({ name, uniformIdx });
		glUseProgram(this->csProgram);
		glUniformMatrix4fv(uniformIdx, 1, GL_FALSE, & value[0][0]);
	}
	checkOpenGLErrors();
}

void ComputeShader::runShader(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, bool block)
{
	//LOG_F(INFO, "Run Compute shader %s with %d", this->m_shaderFileName.c_str(), num_groups_x);
	if (num_groups_x * num_groups_y * num_groups_z <= 0) return;
	glUseProgram(this->csProgram);
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
	if (block) {
		glFinish();
	}
	//Sleep(100);
	checkOpenGLErrors();
}
