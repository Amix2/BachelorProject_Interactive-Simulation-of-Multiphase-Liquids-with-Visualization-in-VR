#include "VRCameraController.h"

VRCameraController::VRCameraController(ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance )
	: eyesDistance{ eyesDistance }
	, position{ glm::vec3{ 0.0f } }
	, front{ glm::vec3{ 1.0f, 0.0f, 0.0f } }
	, up{ glm::vec3{ 0.0f, 1.0f, 0.0f } }
	, right{ glm::vec3{ 0.0f, 0.0f, 1.0f } }
	, rightEye{ rightEyeViewPort, this->position + this->right * eyesDistance * 0.5f }
	, leftEye{ leftEyeViewPort, this->position - this->right * eyesDistance * 0.5f }
{}

void VRCameraController::setHead(const vr::HmdMatrix34_t& view) {
	this->position = glm::vec3(view.m[0][3], view.m[1][3], view.m[2][3]);
	this->front = -glm::vec3(view.m[0][2], view.m[1][2], view.m[2][2]);
	this->up = glm::vec3(view.m[0][1], view.m[1][1], view.m[2][1]);
	this->right = -glm::vec3(view.m[0][0], view.m[1][0], view.m[2][0]);
	leftEye.set(
		this->position,
		this->front,
		this->up,
		this->right
	);

	rightEye.set(
		this->position,
		this->front,
		this->up,
		this->right
	);
}

void VRCameraController::setEyeMatrix(const glm::mat4& eyeMatrix, vr::Hmd_Eye eye)
{
	if (eye == vr::Eye_Left)
		leftEye.eye = eyeMatrix;
	else if (eye == vr::Eye_Right)
		leftEye.eye = eyeMatrix;
}

void VRCameraController::setProjectionMatrix(const glm::mat4& projectionMatrix, vr::Hmd_Eye eye)
{
	if (eye == vr::Eye_Left)
		leftEye.eye = projectionMatrix;
	else if (eye == vr::Eye_Right)
		leftEye.eye = projectionMatrix;
}

const std::vector<const Scene::Camera*> VRCameraController::provideCameras() const
{
	return std::vector<const Scene::Camera*>{ &this->leftEye, &this->rightEye };
}
