#include "GraphicShaderStorage.h"


void GraphicShaderStorage::addShader(std::string name, const ShaderProgram& shader)
{
	m_shaderMap[name] = shader;
}

ShaderProgram GraphicShaderStorage::getShader(std::string name)
{
	return m_shaderMap[name];
}
