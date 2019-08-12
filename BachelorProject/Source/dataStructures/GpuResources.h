#pragma once
#include <GL/glew.h>
#include <unordered_map>

/* keeps all data structures created on GPU */
class GpuResources
{
	static std::unordered_map<std::string, GLuint> m_ResourceMap;
	GpuResources() {}	   // hide constructor

public:

	static void		createSSBO(std::string name, GLsizeiptr size, const void *data, GLuint bindingPointIndex);
	/* download data from gpu to cpu adress space, data in valid until gpu changes it */
	static void*	getDataSSBO(std::string name);
	static GLuint	getIndexSSBO(std::string name);

};
