#include "GpuResources.h"

std::unordered_map<std::string, GLuint> GpuResources::m_ResourceMap;

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
}

void * GpuResources::getDataSSBO(std::string name)
{
	if (GpuResources::m_ResourceMap.find(name) != GpuResources::m_ResourceMap.end()) {

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

