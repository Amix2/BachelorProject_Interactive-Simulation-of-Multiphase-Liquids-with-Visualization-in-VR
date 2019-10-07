#include "VRCameraController.h"

VRCameraController::VRCameraController(ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance, const glm::vec3& position)
	: eyesDistance{ eyesDistance }
	, position{ position }
	, right{ glm::vec3{ 0.0f, 0.0f, 1.0f } }
	, up{ glm::vec3{ 0.0f, 1.0f, 0.0f } }
	, front{ glm::vec3{ 1.0f, 0.0f, 0.0f } }
	, rightEye{ rightEyeViewPort, this->position + this->right * eyesDistance * 0.5f }
	, leftEye{ leftEyeViewPort, this->position - this->right * eyesDistance * 0.5f }
{}

void VRCameraController::setHead(const  vr::HmdMatrix44_t& leftEye, const  vr::HmdMatrix44_t& rightEye)
{
	this->leftEye.set(leftEye);
	this->rightEye.set(rightEye);
}