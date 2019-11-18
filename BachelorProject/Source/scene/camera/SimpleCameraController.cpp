#include "SimpleCameraController.h"

SimpleCameraController::SimpleCameraController(InputDispatcher& inputDispatcher, ViewPort& viewport, const glm::vec3& position)
	: camera{ viewport , position }
{
	inputDispatcher.subscribeForMousePositionChanges(this);
	inputDispatcher.subscribeForMouseScroll(this);
	inputDispatcher.subscribeForKeyInput(this, std::vector<int>{ 
		GLFW_KEY_W,
		GLFW_KEY_S,
		GLFW_KEY_A,
		GLFW_KEY_D,
		GLFW_KEY_Q,
		GLFW_KEY_E,
		GLFW_MOUSE_BUTTON_LEFT
	});

}


void SimpleCameraController::handleMouseMove(float xoffset, float yoffset)
{
	if (isLocked)
		return;
	xoffset *= MOUSE_SENSITIVITY;
	yoffset *= MOUSE_SENSITIVITY;

	float cameraPitch = camera.getPitch();
	float constrainedPitch =
		cameraPitch + (float)yoffset > 89.0f ? 89.0f :
		cameraPitch + (float)yoffset < -89.0f ? -89.0f :
		cameraPitch + (float)yoffset;
		
	camera.setRotation(constrainedPitch, camera.getYaw() + (float)xoffset, camera.getRoll());
	
}

void SimpleCameraController::handleMouseScroll(float scroll)
{
	float cameraZoom = camera.getZoom();
	float constrianedZoom =
		cameraZoom + scroll > 60.0f ? 60.0f :
		cameraZoom + scroll < 1.0f ? 1.0f :
		cameraZoom + scroll;
	camera.setZoom(constrianedZoom);
}

void SimpleCameraController::handleKeyPress(int key, KeyState action, float deltaTime)
{
	if (action == KeyState::PRESSED) {
		switch (key) {
		case GLFW_KEY_W:
			camera.setPosition(camera.getPosition() + camera.getFront() * deltaTime * VELOCITY);
			break;
		case GLFW_KEY_S:
			camera.setPosition(camera.getPosition() - camera.getFront() * deltaTime * VELOCITY);
			break;
		case GLFW_KEY_A:
			camera.setPosition(camera.getPosition() - camera.getRight() * deltaTime * VELOCITY);
			break;
		case GLFW_KEY_D:
			camera.setPosition(camera.getPosition() + camera.getRight() * deltaTime * VELOCITY);
			break;
		case GLFW_KEY_Q:
			camera.setPosition(camera.getPosition() + glm::vec3(0, 1, 0) * deltaTime * VELOCITY);
			break;
		case GLFW_KEY_E:
			camera.setPosition(camera.getPosition() - glm::vec3(0, 1, 0) * deltaTime * VELOCITY);
			break;
		default:
			break;
		}
	}
	else if (action == KeyState::FALLING_EDGE) {
	}
		
	if (action == KeyState::FALLING_EDGE && key == GLFW_MOUSE_BUTTON_LEFT) {
		isLocked = true;
	}

	if (action == KeyState::RISING_EDGE && key == GLFW_MOUSE_BUTTON_LEFT) {
		isLocked = false;
	}
}

const std::vector<const Scene::Camera*> SimpleCameraController::provideCameras() const
{
	return std::vector<const Scene::Camera*>{ &this->camera };
}
