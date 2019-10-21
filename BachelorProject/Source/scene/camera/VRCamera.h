#pragma once
#include "Camera.h"
#include "VRCameraController.h"

class VRCamera :
	public Scene::Camera
{
public:
	VRCamera(ViewPort& viewPort);
	friend VRCameraController;

	glm::mat4 getViewMatrix() const override;
	glm::mat4 getProjectionMatrix() const override;
private:
	glm::mat4 projection;
	glm::mat4 eye;
};

