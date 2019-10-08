#include "SimpleCameraController.h"

SimpleCameraController::SimpleCameraController(Window& window, ViewPort& viewport, const glm::vec3& position)
	: camera{ viewport , position }
	, window{ &window }
{
	window.subscribeForMousePositionChanges(this);
	window.subscribeForMouseScrollChanges(this);
	window.subscribeForKeyInput(this);
	cam = &camera;
}

SimpleCameraController::~SimpleCameraController()
{
	window->unsubscribeMousePositionListener(this);
	window->unsubscribeMouseScrollListener(this);
	window->unsubscribeKeyInputListener(this);
}

void SimpleCameraController::handleMouseMove(double xoffset, double yoffset)
{
	xoffset *= MOUSE_SENSITIVITY;
	yoffset *= MOUSE_SENSITIVITY;

	float cameraPitch = camera.getPitch();
	float constrainedPitch =
		cameraPitch + (float)yoffset > 89.0f ? 89.0f :
		cameraPitch + (float)yoffset < -89.0f ? -89.0f :
		cameraPitch + (float)yoffset;
		
	camera.setRotation(constrainedPitch, camera.getYaw() + (float)xoffset, camera.getRoll());
}

void SimpleCameraController::handleMouseScroll(double scroll)
{
	float cameraZoom = camera.getZoom();
	float constrianedZoom =
		cameraZoom + scroll > 60.0f ? 60.0f :
		cameraZoom + scroll < 1.0f ? 1.0f :
		cameraZoom + scroll;
	camera.setZoom(constrianedZoom);
}

void SimpleCameraController::handleKeyPress(Key key)
{
	float deltaTime = window->getDeltaTime();
	switch (key) {
	case KEY_W:
		camera.setPosition(camera.getPosition() + camera.getFront() * deltaTime * VELOCITY);
		break;
	case KEY_S:
		camera.setPosition(camera.getPosition() - camera.getFront() * deltaTime * VELOCITY);
		break;
	case KEY_A:
		camera.setPosition(camera.getPosition() - camera.getRight() * deltaTime * VELOCITY);
		break;
	case KEY_D:
		camera.setPosition(camera.getPosition() + camera.getRight() * deltaTime * VELOCITY);
		break;
	case KEY_Q:
		camera.setPosition(camera.getPosition() + glm::vec3(0, 1, 0) * deltaTime * VELOCITY);
		break;
	case KEY_E:
		camera.setPosition(camera.getPosition() - glm::vec3(0, 1, 0) * deltaTime * VELOCITY);
		break;
	default:
		break;
	}
}
