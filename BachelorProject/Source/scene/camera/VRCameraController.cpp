#include "VRCameraController.h"

VRCameraController::VRCameraController(ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance )
	: eyesDistance{ eyesDistance }
	, position{ glm::vec3{ 0.0f } }
	, front{ glm::vec3{ 1.0f, 0.0f, 0.0f } }
	, up{ glm::vec3{ 0.0f, 1.0f, 0.0f } }
	, right{ glm::vec3{ 0.0f, 0.0f, 1.0f } }
	, rightEye{ rightEyeViewPort}
	, leftEye{ leftEyeViewPort }
{}

void VRCameraController::setHead(const vr::HmdMatrix34_t& view) {
	this->position = (glm::vec3(view.m[0][3], view.m[1][3], view.m[2][3])) * MULTIPLYER;
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

#include <glm/gtx/string_cast.hpp>
void VRCameraController::setEyeMatrix(const glm::mat4& eyePoseMatrix, vr::Hmd_Eye eye)
{
	if (eye == vr::Eye_Left) {
		leftEye.eye = eyePoseMatrix;
		leftEye.eye[3][0] *= MULTIPLYER;
		leftEye.eye[3][1] *= MULTIPLYER;
		leftEye.eye[3][2] *= MULTIPLYER;
	}
	else if (eye == vr::Eye_Right) {
		rightEye.eye = eyePoseMatrix;
		rightEye.eye[3][0] *= MULTIPLYER;
		rightEye.eye[3][1] *= MULTIPLYER;
		rightEye.eye[3][2] *= MULTIPLYER;
	}
}

void VRCameraController::setProjectionMatrix(const glm::mat4& projectionMatrix, vr::Hmd_Eye eye)
{
	if (eye == vr::Eye_Left)
		leftEye.projection = projectionMatrix;
	else if (eye == vr::Eye_Right)
		rightEye.projection = projectionMatrix;
}

const std::vector<const Scene::Camera*> VRCameraController::provideCameras() const
{
	return std::vector<const Scene::Camera*>{ &this->leftEye, &this->rightEye };
}
