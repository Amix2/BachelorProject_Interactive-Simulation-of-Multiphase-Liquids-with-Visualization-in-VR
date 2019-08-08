#include "ComputeShader.h"

ComputeShader::ComputeShader(const std::string shaderFileName)
{
	GLuint csCreatorID = glCreateShader(GL_COMPUTE_SHADER);

	std::ifstream csInputFileStream(shaderFileName);
	const std::string computeShaderCode((std::istreambuf_iterator<char>(csInputFileStream)), std::istreambuf_iterator<char>());

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

		GLchar* strInfoLog = new GLchar[length + 1];
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
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	if (block) {
		glFinish();
	}
}

void ComputeShader::bindSSBO(std::string name, GLsizeiptr size, const void * data, GLuint bindingPointIndex) const
{
	GLuint ssbo;

	glGenBuffers(1, &ssbo);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glNamedBufferData(ssbo, size, data, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);		 // unbind

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPointIndex, ssbo);

	GLuint block_index = 0;
	block_index = glGetProgramResourceIndex(this->csProgram, GL_SHADER_STORAGE_BLOCK, name.c_str());
	glShaderStorageBlockBinding(this->csProgram, block_index, bindingPointIndex);

	this->bufferMap[name] = ssbo;
}

void * ComputeShader::getSSBO(std::string name)
{
	if (this->bufferMap.find(name) != this->bufferMap.end()) {

		GLuint ssbo = this->bufferMap[name];
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		void* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		return p;
	} else {
		throw "no SSBO for given name";
	}
}
