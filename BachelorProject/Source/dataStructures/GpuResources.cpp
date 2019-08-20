#include "GpuResources.h"

//////////////////////
//	SSBO

void GpuResources::createSSBO(std::string name, GLsizeiptr size, const void * data, GLuint bindingPointIndex)
{
	GLuint ssbo;

	glGenBuffers(1, &ssbo);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glNamedBufferData(ssbo, size, data, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);		 // unbind

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPointIndex, ssbo);

	// store ssbo ID in local map
	GpuResources::m_ResourceMap.insert({ name, ssbo });

	LOG_F(INFO, "new SSBO, name: %s, \tid: %d", name.c_str(), ssbo);
}

void * GpuResources::getDataSSBO(std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {	// if contsins

		GLuint ssbo = GpuResources::m_ResourceMap[name];

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		void* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		return p;
	}
	else {
		throw "no SSBO for given name";
	}
}

void * GpuResources::openSSBO(std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {	// if contsins

		GLuint ssbo = GpuResources::m_ResourceMap[name];

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		m_openResourceName = name;
		return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	}
	else {
		throw "no SSBO for given name";
	}
}

void* GpuResources::openPartSSBO(std::string name, GLintptr offset, GLsizeiptr length)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {	// if contsins

		GLuint ssbo = GpuResources::m_ResourceMap[name];

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		m_openResourceName = name;
		return glMapNamedBufferRange(ssbo, offset, length, GL_MAP_WRITE_BIT);

	}
	else {
		throw "no SSBO for given name";
	}
}

void GpuResources::commitSSBO(std::string name)
{
	if(m_openResourceName != name) throw "wrong SSBO name, different one was opened";
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

GLuint GpuResources::getIndexSSBO(std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {
		return GpuResources::m_ResourceMap[name];
	}
	else {
		throw "no SSBO for given name";
	}
}

//	-end- SSBO
//////////////////////

