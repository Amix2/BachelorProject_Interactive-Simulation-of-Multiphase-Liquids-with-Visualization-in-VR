#include "HandDataProvider.h"


glm::mat4 HandDataProvider::getLeftHandMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(1,0,1));
}

glm::mat4 HandDataProvider::getRightHandMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(0,0,1));
}
