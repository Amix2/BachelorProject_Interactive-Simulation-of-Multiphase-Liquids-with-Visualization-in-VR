#pragma once
#include <glm/glm.hpp>
enum class Hand;

class DataProvider
{
	glm::mat4 getMatrix(Hand hand);

};

