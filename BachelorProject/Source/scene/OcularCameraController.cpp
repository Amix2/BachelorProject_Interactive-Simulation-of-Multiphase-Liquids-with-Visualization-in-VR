#include "OcularCameraController.h"

OcularCameraController::OcularCameraController(Window& window, Scene::Camera& leftEye, Scene::Camera& rightEye)
	:leftEye{ &leftEye }
	,rightEye{ &rightEye }
{
	window.subscribeForMousePositionChanges(this);
	window.subscribeForMouseScrollChanges(this);
}

void OcularCameraController::handleMouseMove(double xoffset, double yoffset)
{
	xoffset *= MOUSE_SENSITIVITY;
	yoffset *= MOUSE_SENSITIVITY;

	rightEye->changeYaw(xoffset);
	rightEye->changePitch(yoffset);

	leftEye->changeYaw(xoffset);
	leftEye->changePitch(yoffset);
}

void OcularCameraController::handleMouseScroll(double scroll)
{
}
