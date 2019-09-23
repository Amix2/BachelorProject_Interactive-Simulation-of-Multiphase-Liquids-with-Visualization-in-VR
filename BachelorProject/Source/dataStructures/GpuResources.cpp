#include "GpuResources.h"

//////////////////////
//	GENERAL

GLuint GpuResources::createResource(GLenum target, std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex)
{
	GLuint index;

	glGenBuffers(1, &index);

	glBindBufferBase(target, bindingPointIndex, index);
	glBindBuffer(target, index);
	glNamedBufferData(index, size, data, GL_DYNAMIC_READ);

	checkOpenGLErrors();

	// store resource ID in local map
	GpuResources::m_NamesMap.insert({ name, index });
	GpuResources::m_SizesMap.insert({ name, size });

	return index;
}

void* GpuResources::getDataResource(GLenum target, std::string name)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {	// if contains

		GLuint index = GpuResources::m_NamesMap[name];

		glBindBuffer(target, index);
		void* p = glMapBuffer(target, GL_READ_WRITE);
		glUnmapBuffer(target);

		return p;
	}
	else {
		LOG_F(ERROR, "no Resource for given name");
		throw "no SSBO for given name";
	}
}

void* GpuResources::openResource(GLenum target, std::string name)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {	// if contains

		GLuint index = GpuResources::m_NamesMap[name];
		GLsizeiptr length = GpuResources::m_SizesMap[name];

		glBindBuffer(target, index);
		void* p = glMapBuffer(target, GL_READ_WRITE);
		//void* p = glMapNamedBufferRange(index, 0, length, GL_MAP_UNSYNCHRONIZED_BIT  | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT);
		checkOpenGLErrors();
		return p;
	}
	else {
		LOG_F(ERROR, "no Resource for given name");
		throw "no SSBO for given name";
	}
}

void* GpuResources::openPartResource(GLenum target, std::string name, GLintptr offset, GLsizeiptr length)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {	// if contsins

		GLuint index = GpuResources::m_NamesMap[name];

		glBindBuffer(target, index);
		LOG_F(INFO, "open part %s, offset %d, length %d", name.c_str(),  (int)offset, (int)length);
		void* ptr = glMapNamedBufferRange(index, offset, length, GL_MAP_WRITE_BIT);
		checkOpenGLErrors();
		return ptr;

	}
	else {
		LOG_F(ERROR, "no Resource for given name");
		throw "no SSBO for given name";
	}
}

void GpuResources::commitResource(GLenum target, std::string name)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {	// if contsins

		GLuint index = GpuResources::m_NamesMap[name];
		glBindBuffer(target, index);
		glUnmapBuffer(target);
		checkOpenGLErrors();
	}
	else {
		LOG_F(ERROR, "no Resource for given name");
		throw "no SSBO for given name";
	}
}

void GpuResources::attachResource(GLenum target, GLuint bindingPointIndex, GLuint resourceIndexName)
{
	glBindBufferBase(target, bindingPointIndex, resourceIndexName);
	checkOpenGLErrors();
}

void GpuResources::copyResourceSubData(GLuint source, GLuint target, GLintptr sourceOffset, GLintptr targetOffset, GLsizeiptr size)
{
	//LOG_F(INFO, "CPY Resource SubData from %d ::%d to %d ::%d, size: %d", source, sourceOffset, target, targetOffset, size);

	glCopyNamedBufferSubData(source, target, sourceOffset, targetOffset, size);
	//LOG_F(INFO, " 1sd");
	checkOpenGLErrors();
}

//////////////////////
//	SSBO

void GpuResources::createSSBO(std::string name, GLsizeiptr size, const void * data, GLuint bindingPointIndex)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {
		// resource already made, attach it
		GLuint index = GpuResources::m_NamesMap[name];
		attachResource(GL_SHADER_STORAGE_BUFFER, bindingPointIndex, index);
		LOG_F(INFO, "SSBO attached, name: %s, \tid: %d", name.c_str(), index);
	}
	else {
		// resource not yet made
		GLuint ssbo = createResource(GL_SHADER_STORAGE_BUFFER, name, size, data, bindingPointIndex);
		LOG_F(INFO, "new SSBO, name: %s, \tid: %d", name.c_str(), ssbo);
	}

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

void GpuResources::attachSSBO(std::string name, GLuint bindingPointIndex)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {	// if contsins

		GLuint index = GpuResources::m_NamesMap[name];
		attachResource(GL_SHADER_STORAGE_BUFFER, bindingPointIndex, index);
		checkOpenGLErrors();
	}
	else {
		LOG_F(ERROR, "no Resource for given name");
		throw "no SSBO for given name";
	}
}


//	-end- SSBO
//////////////////////

//////////////////////
//	UBO


void GpuResources::createUBO(std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {
		// resource already made, attach it
		GLuint index = GpuResources::m_NamesMap[name];
		attachResource(GL_UNIFORM_BUFFER, bindingPointIndex, index);
		LOG_F(INFO, "UBO attached, name: %s, \tid: %d", name.c_str(), index);
	}
	else {
		GLuint ubo = createResource(GL_UNIFORM_BUFFER, name, size, data, bindingPointIndex);

		LOG_F(INFO, "new UBO, name: %s, \tid: %d", name.c_str(), ubo);
	}
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

void GpuResources::attachUBO(std::string name, GLuint bindingPointIndex)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {	// if contsins

		GLuint index = GpuResources::m_NamesMap[name];
		attachResource(GL_UNIFORM_BUFFER, bindingPointIndex, index);
		checkOpenGLErrors();
	}
	else {
		LOG_F(ERROR, "no Resource for given name");
		throw "no UBO for given name";
	}
}

void GpuResources::setAsCopySource(std::string sourceName)
{
	glBindBuffer(GL_COPY_READ_BUFFER, m_NamesMap[sourceName]);
	checkOpenGLErrors();
}

void GpuResources::setAsCopyTarget(std::string targetName)
{
	glBindBuffer(GL_COPY_WRITE_BUFFER, m_NamesMap[targetName]);
	checkOpenGLErrors();
}

void GpuResources::copyResourceSubData(std::string source, std::string target, GLintptr sourceOffset, GLintptr targetOffset, GLsizeiptr size)
{
	//LOG_F(INFO, "CPY Resource SubData from %s ::%d to %s ::%d, size: %d", source.c_str(), sourceOffset, target.c_str(), targetOffset, size);
	if (GpuResources::m_NamesMap.find(source) == GpuResources::m_NamesMap.end()
		|| GpuResources::m_NamesMap.find(target) == GpuResources::m_NamesMap.end()) {
		LOG_F(ERROR, "no Resource for given name");
		throw "no Resource for given name";
	}
	GpuResources::copyResourceSubData(m_NamesMap[source], m_NamesMap[target], sourceOffset, targetOffset, size);
}


//	-end- UBO
//////////////////////


GLuint GpuResources::getIndex(std::string name)
{
	if (GpuResources::m_NamesMap.find(name) != GpuResources::m_NamesMap.end()) {
		return GpuResources::m_NamesMap[name];
	}
	else {
		throw "no SSBO for given name";
	}
}
