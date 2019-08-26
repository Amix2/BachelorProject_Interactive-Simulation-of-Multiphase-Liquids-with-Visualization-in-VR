#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include <Logger.h>
#include <Utils.h>

/* keeps all data structures created on GPU */
class GpuResources
{
	// map with bufferIDs
	inline static std::unordered_map<std::string, GLuint> m_ResourceMap;

	static GLuint	createResource(GLenum target, std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex);

	static void*	getDataResource(GLenum target, std::string name);

	static void*	openResource(GLenum target, std::string name);

	static void*	openPartResource(GLenum target, std::string name, GLintptr offset, GLsizeiptr length);

	static void		commitResource(GLenum target, std::string name);
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

	// create UBO and add its name to local map
	static void		createUBO(std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex);

	// download data from gpu to cpu address space, data in up to date but cannot be changed, to change data use open & commit
	static void*	getDataUBO(std::string name);

	// download data from gpu to cpu address space, keep it opened untill commitSSBO() call
	static void*	openUBO(std::string name);

	// download data with offset and length from gpu to cpu address space, keep it opened untill commitUBO() call
	static void*	openPartUBO(std::string name, GLintptr offset, GLsizeiptr length);

	// commits memory changes in cup adress space and sends them to gpu
	static void		commitUBO(std::string name);



	// gets a gpu index of buffer
	static GLuint	getIndex(std::string name);

};
