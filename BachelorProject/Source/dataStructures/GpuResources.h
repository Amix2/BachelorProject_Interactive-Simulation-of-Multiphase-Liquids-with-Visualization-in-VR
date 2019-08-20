#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include <Logger.h>

/* keeps all data structures created on GPU */
class GpuResources
{
	// map with bufferIDs
	inline static std::unordered_map<std::string, GLuint> m_ResourceMap;

	// name of last opened resource, used in commit checks
	inline static std::string m_openResourceName;

public:
	// create SSBO and add its name to local map
	static void		createSSBO(std::string name, GLsizeiptr size, const void *data, GLuint bindingPointIndex);

	// download data from gpu to cpu address space, data in up to date but cannot be changed, to change data use open & commit
	static void*	getDataSSBO(std::string name);

	// download data from gpu to cpu address space, keep it opened untill commitSSBO() call
	static void*	openSSBO(std::string name);

	// download data with offset and length from gpu to cpu address space, keep it opened untill commitSSBO() call
	static void*	openPartSSBO(std::string name, GLintptr offset, GLsizeiptr length);

	// commits memory changes in cup adress space and sends them to gpu
	static void		commitSSBO(std::string name);

	// gets a gpu index of ssbo buffer
	static GLuint	getIndexSSBO(std::string name);

};
