#pragma once
#include <glm/glm.hpp>


class HandDataProvider
{
public:
	HandDataProvider() {}

	glm::mat4 getLeftHandMatrix();
	glm::mat4 getRightHandMatrix();
};

