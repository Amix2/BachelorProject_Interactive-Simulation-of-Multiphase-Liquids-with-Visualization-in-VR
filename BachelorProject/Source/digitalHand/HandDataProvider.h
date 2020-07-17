#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class HandDataProvider
{
public:
	HandDataProvider() {}

	glm::mat4 getLeftHandMatrix();
	glm::mat4 getRightHandMatrix();
};

