#include "SimpleCameraController.h"

SimpleCameraController::SimpleCameraController(Window& window, ViewPort& viewport, const glm::vec3& position)
	: camera{ viewport , position }
	, window{ &window }
{
	window.subscribeForMousePositionChanges(this);
	window.subscribeForMouseScrollChanges(this);
	window.subscribeForKeyInput(this);
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

	camera.setRotation(camera.getPitch() + (float)yoffset, camera.getYaw() + (float)xoffset, camera.getRoll());
}

void SimpleCameraController::handleMouseScroll(double scroll)
{
	//TODO: Zoom
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
		camera.setPosition(camera.getPosition() + camera.getUp() * deltaTime * VELOCITY);
		break;
	case KEY_E:
		camera.setPosition(camera.getPosition() - camera.getUp() * deltaTime * VELOCITY);
		break;
	default:
		break;
	}
}
