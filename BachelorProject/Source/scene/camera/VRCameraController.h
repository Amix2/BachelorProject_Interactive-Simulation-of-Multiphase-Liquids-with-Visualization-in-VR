#pragma once

#include <window/Window.h>
#include "Camera.h"
#include <glm/glm.hpp>
#include <OpenVR/openvr.h>

class VRCameraController

{
public:
	VRCameraController(ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance);
	
	void setHead(const vr::HmdMatrix34_t& eye);
	Scene::Camera& getRightCamera() { return rightEye; }
	Scene::Camera& getLeftCamera() { return leftEye; }
private:
	float eyesDistance;
	glm::vec3 headUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	Scene::Camera leftEye;
	Scene::Camera rightEye;

};