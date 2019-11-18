#pragma once
#include <unordered_map>
#include <utilities/shader/ShaderProgram.h>

const struct ShaderNames {
	std::string GlassObject = "GlassObject";
	std::string SelectedGlassObject = "SelectedGlassObject";
}ShaderNames;


class GraphicShaderStorage
{
private:
	static inline std::unordered_map<std::string, ShaderProgram> m_shaderMap;
public:
	static void addShader(std::string name, const ShaderProgram &shader);
	static ShaderProgram getShader(std::string name);
};

