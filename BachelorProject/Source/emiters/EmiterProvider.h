#pragma once
#include <glm/mat4x4.hpp>

class EmiterProvider
{
public:
	virtual glm::mat4 getEmiterMatrix() const = 0;
};

