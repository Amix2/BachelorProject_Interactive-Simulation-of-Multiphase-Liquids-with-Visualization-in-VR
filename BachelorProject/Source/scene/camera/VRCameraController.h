#pragma once

#include "../../window/listener/MousePositionListener.h"
#include "../../window/listener/MouseScrollListener.h"
#include "../../window/listener/WindowSizeListener.h"
#include "../../window/Window.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <OpenVR/openvr.h>

class VRCameraController
{
public:
	VRCameraController(ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance, const glm::vec3& position);
	
	void setHead(const vr::HmdMatrix44_t& leftEye, const vr::HmdMatrix44_t& rightEye);
	Scene::Camera& getRightCamera() { return rightEye; }
	Scene::Camera& getLeftCamera() { return leftEye; }
private:
	inline static const glm::vec3 WORLD_UP{ 0.0f, 1.0f, 0.0f };

	float eyesDistance;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	Scene::Camera leftEye;
	Scene::Camera rightEye;

};