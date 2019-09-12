#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include <Logger.h>
#include <Utils.h>

/* keeps all data structures created on GPU
create resource -> initialize array on gpu and store its ID in local map
open resource -> map gpu array (buffer object) to cpu adress space
commit resource -> unmap array, sends data from cpu to gpu 
get data (open & commit) -> returns array (debug only)
*/
class GpuResources
{
	// map with bufferIDs
	inline static std::unordered_map<std::string, GLuint> m_NamesMap;
	inline static std::unordered_map<std::string, GLsizeiptr> m_SizesMap;

	static GLuint	createResource(GLenum target, std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex);

	static void*	getDataResource(GLenum target, std::string name);

	static void*	openResource(GLenum target, std::string name);

	static void*	openPartResource(GLenum target, std::string name, GLintptr offset, GLsizeiptr length);

	static void		commitResource(GLenum target, std::string name);

	static void		attachResource(GLenum target, GLuint bindingPointIndex, GLuint resourceIndexName);
public:

	static void		createSSBO(std::string name, GLsizeiptr size, const void *data, GLuint bindingPointIndex);	// create SSBO and add its name to local map
	static void*	getDataSSBO(std::string name);	// download data from gpu to cpu address space, data in up to date but cannot be changed, to change data use open & commit
	static void*	openSSBO(std::string name);	// download data from gpu to cpu address space, keep it opened untill commitSSBO() call
	static void*	openPartSSBO(std::string name, GLintptr offset, GLsizeiptr length);	// download data with offset and length from gpu to cpu address space, keep it opened untill commitSSBO() call
	static void		commitSSBO(std::string name);	// commits memory changes in cpu adress space and sends them to gpu
	static void		attachSSBO(std::string name, GLuint bindingPointIndex);	// bind existing SSBO to this context

	static void		createUBO(std::string name, GLsizeiptr size, const void* data, GLuint bindingPointIndex);	// create UBO and add its name to local map
	static void*	getDataUBO(std::string name);	// download data from gpu to cpu address space, data in up to date but cannot be changed, to change data use open & commit
	static void*	openUBO(std::string name);	// download data from gpu to cpu address space, keep it opened untill commitSSBO() call
	static void*	openPartUBO(std::string name, GLintptr offset, GLsizeiptr length);	// download data with offset and length from gpu to cpu address space, keep it opened untill commitUBO() call
	static void		commitUBO(std::string name);	// commits memory changes in cpu adress space and sends them to gpu
	static void		attachUBO(std::string name, GLuint bindingPointIndex);	// bind existing UBO to this context


	// gets a gpu index of buffer
	static GLuint	getIndex(std::string name);

};
