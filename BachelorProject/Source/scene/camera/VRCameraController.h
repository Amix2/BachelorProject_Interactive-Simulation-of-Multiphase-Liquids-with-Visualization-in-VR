#pragma once

#include <window/Window.h>
#include "VRCamera.h"
#include <glm/glm.hpp>
#include <OpenVR/openvr.h>
#include "CameraController.h"


class VRCameraController
	: public CameraController
{
public:
	VRCameraController(ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance);
	
	void setHead(const vr::HmdMatrix34_t& view);
	void setEyeMatrix(const glm::mat4& eyePoseMatrix, vr::Hmd_Eye eye);
	void setProjectionMatrix(const glm::mat4& projectionMatrix, vr::Hmd_Eye eye);

	const std::vector<const Scene::Camera*> provideCameras() const override;

private:
	inline static const float MULTIPLYER = 100.f;

	float eyesDistance;
	glm::vec3 headUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	VRCamera leftEye;
	VRCamera rightEye;

};