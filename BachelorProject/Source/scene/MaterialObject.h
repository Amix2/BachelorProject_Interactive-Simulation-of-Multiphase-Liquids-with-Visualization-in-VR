#pragma once

#include "ShaderProgram.h"
#include <glm/glm.hpp>

class MaterialObject
{
public:
	void virtual load(const glm::mat4& view, const glm::mat4& projection) const = 0;
protected:
	MaterialObject(ShaderProgram shaderProgram);
	ShaderProgram shaderProgram;
private:
};

