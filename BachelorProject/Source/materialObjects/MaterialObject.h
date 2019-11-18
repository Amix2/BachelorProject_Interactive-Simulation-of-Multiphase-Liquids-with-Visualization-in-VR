#pragma once

#include "../utilities/shader/ShaderProgram.h"
#include <glm/glm.hpp>

class MaterialObject
{
public:
	void virtual init() = 0;
	void virtual load(const glm::mat4& view, const glm::mat4& projection) const = 0;

	bool render = true;
	void toggleRender() { render = !render; }
protected:
	MaterialObject(ShaderProgram shaderProgram);
	ShaderProgram shaderProgram;
private:
};

