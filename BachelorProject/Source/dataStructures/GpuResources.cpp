#include "GpuResources.h"

//////////////////////
//	GENERAL
GLuint GpuResources::createResource(GLenum target, std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex)
{
	GLuint index;

	glGenBuffers(1, &index);

	glBindBufferBase(target, bindingPointIndex, index);
	glBindBuffer(target, index);
	glNamedBufferData(index, size, data, GL_DYNAMIC_COPY);

	checkOpenGLErrors();

	// store resource ID in local map
	GpuResources::m_ResourceMap.insert({ name, index });

	return index;
}

void* GpuResources::getDataResource(GLenum target, std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {	// if contains

		GLuint index = GpuResources::m_ResourceMap[name];

		glBindBuffer(target, index);
		void* p = glMapBuffer(target, GL_WRITE_ONLY);
		glUnmapBuffer(target);

		return p;
	}
	else {
		throw "no SSBO for given name";
	}
}

void* GpuResources::openResource(GLenum target, std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {	// if contains

		GLuint index = GpuResources::m_ResourceMap[name];
		glBindBuffer(target, index);
		void* p = glMapBuffer(target, GL_WRITE_ONLY);
		return p;
	}
	else {
		throw "no SSBO for given name";
	}
}

void* GpuResources::openPartResource(GLenum target, std::string name, GLintptr offset, GLsizeiptr length)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {	// if contsins

		GLuint index = GpuResources::m_ResourceMap[name];

		glBindBuffer(target, index);
		//LOG_F(INFO, "open part, offset %d, length %d", (int)offset, (int)length);
		void* ptr = glMapNamedBufferRange(index, offset, length, GL_MAP_WRITE_BIT);
		checkOpenGLErrors();
		return ptr;

	}
	else {
		throw "no SSBO for given name";
	}
}

void GpuResources::commitResource(GLenum target, std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {	// if contsins

		GLuint index = GpuResources::m_ResourceMap[name];
		glBindBuffer(target, index);
		glUnmapBuffer(target);
	}
	else {
		throw "no SSBO for given name";
	}
}

//////////////////////
//	SSBO

void GpuResources::createSSBO(std::string name, GLsizeiptr size, const void * data, GLuint bindingPointIndex)
{
	GLuint ssbo = createResource(GL_SHADER_STORAGE_BUFFER, name, size, data, bindingPointIndex);

	LOG_F(INFO, "new SSBO, name: %s, \tid: %d", name.c_str(), ssbo);
}

void * GpuResources::getDataSSBO(std::string name)
{
	return getDataResource(GL_SHADER_STORAGE_BUFFER, name);
}

void * GpuResources::openSSBO(std::string name)
{
	return openResource(GL_SHADER_STORAGE_BUFFER, name);
}

void* GpuResources::openPartSSBO(std::string name, GLintptr offset, GLsizeiptr length)
{
	return openPartResource(GL_SHADER_STORAGE_BUFFER, name, offset, length);
}

void GpuResources::commitSSBO(std::string name)
{
	commitResource(GL_SHADER_STORAGE_BUFFER, name);
}


//	-end- SSBO
//////////////////////

//////////////////////
//	UBO


void GpuResources::createUBO(std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex)
{
	GLuint ssbo = createResource(GL_UNIFORM_BUFFER, name, size, data, bindingPointIndex);

	LOG_F(INFO, "new UBO, name: %s, \tid: %d", name.c_str(), ssbo);
}

void* GpuResources::getDataUBO(std::string name)
{
	return getDataResource(GL_UNIFORM_BUFFER, name);
}

void* GpuResources::openUBO(std::string name)
{
	return openResource(GL_UNIFORM_BUFFER, name);
}

void* GpuResources::openPartUBO(std::string name, GLintptr offset, GLsizeiptr length)
{
	return openPartResource(GL_UNIFORM_BUFFER, name, offset, length);
}

void GpuResources::commitUBO(std::string name)
{
	commitResource(GL_UNIFORM_BUFFER, name);
}

//	-end- UBO
//////////////////////


GLuint GpuResources::getIndex(std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {
		return GpuResources::m_ResourceMap[name];
	}
	else {
		throw "no SSBO for given name";
	}
}
