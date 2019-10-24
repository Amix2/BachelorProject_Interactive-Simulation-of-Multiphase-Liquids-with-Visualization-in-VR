#include "VRCamera.h"

VRCamera::VRCamera(ViewPort& viewPort)
	: Scene::Camera(viewPort, glm::vec3{ 0.0f })
	, projection{ glm::mat4{ 0.0f } }
	, eye{ glm::mat4{ 0.0f } } {}

#include <glm/gtx/string_cast.hpp>
glm::mat4 VRCamera::getViewMatrix() const
{
	//std::cout << "V" << std::endl;

	//std::cout << glm::to_string(Scene::Camera::getViewMatrix()) << std::endl;
	//std::cout << "E" << std::endl;

	//std::cout << glm::to_string(eye) << std::endl;

	return eye * Scene::Camera::getViewMatrix();
}

glm::mat4 VRCamera::getProjectionMatrix() const
{
	//std::cout << "P" << std::endl;
	//std::cout << glm::to_string(projection) << std::endl;

	return projection;
}


