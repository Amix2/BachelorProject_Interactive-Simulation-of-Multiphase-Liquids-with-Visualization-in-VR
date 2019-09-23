#include "OcularCameraController.h"

OcularCameraController::OcularCameraController(Window& window, ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance, const glm::vec3& position)
	:eyesDistance{ eyesDistance }
	,pitch{ DEFAULT_CAM_PITCH }
	,yaw{ DEFAULT_CAM_YAW }
	,roll{ DEFAULT_CAM_ROLL }
	,position{ position }
	,rightEye{ rightEyeViewPort, this->position + this->right * eyesDistance * 0.5f }
	,leftEye{ leftEyeViewPort, this->position - this->right * eyesDistance * 0.5f }
{
	updateHeadVectors();

	window.subscribeForMousePositionChanges(this);
	window.subscribeForMouseScrollChanges(this);
}

void OcularCameraController::handleMouseMove(double xoffset, double yoffset)
{
	yaw += xoffset * MOUSE_SENSITIVITY;
	pitch += yoffset * MOUSE_SENSITIVITY;
	updateHeadVectors();

	leftEye.set(
		this->position - this->right * eyesDistance * 0.5f,
		this->front,
		this->up,
		this->right
	);

	rightEye.set(
		this->position + this->right * eyesDistance * 0.5f,
		this->front,
		this->up,
		this->right
	);
}

void OcularCameraController::handleMouseScroll(double scroll)
{
}

void OcularCameraController::setHead(const  vr::HmdMatrix34_t& leftEye, const  vr::HmdMatrix34_t& rightEye)
{
	this->leftEye.set(leftEye);
	this->rightEye.set(rightEye);
}

void OcularCameraController::setHead(const vr::HmdMatrix34_t& eye) {
	this->position = glm::vec3(eye.m[0][3], eye.m[1][3], eye.m[2][3]);
	this->front = -glm::vec3(eye.m[0][2], eye.m[1][2], eye.m[2][2]);
	this->up = glm::vec3(eye.m[0][1], eye.m[1][1], eye.m[2][1]);
	this->right = -glm::vec3(eye.m[0][0], eye.m[1][0], eye.m[2][0]);
	leftEye.set(
		this->position - this->right * eyesDistance * 0.5f,
		this->front,
		this->up,
		this->right
	);

	rightEye.set(
		this->position + this->right * eyesDistance * 0.5f,
		this->front,
		this->up,
		this->right
	);
}

void OcularCameraController::updateHeadVectors()
{
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	headUp = glm::normalize(glm::rotate(WORLD_UP, glm::radians(roll), front));

	right = glm::normalize(glm::cross(front, headUp));

	up = glm::normalize(glm::cross(right, front));
}
