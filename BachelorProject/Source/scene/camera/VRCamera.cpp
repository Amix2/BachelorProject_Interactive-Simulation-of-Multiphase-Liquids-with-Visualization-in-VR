#include "VRCamera.h"

VRCamera::VRCamera(ViewPort& viewPort)
	: Scene::Camera(viewPort, glm::vec3{ 0.0f })
	, projection{ glm::mat4{ 0.0f } }
	, eye{ glm::mat4{ 0.0f } } {}

glm::mat4 VRCamera::getViewMatrix() const
{
	return eye * Scene::Camera::getViewMatrix();
}

glm::mat4 VRCamera::getProjectionMatrix() const
{
	return projection;
}


