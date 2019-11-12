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
		GLFW_KEY_0,	// togle emiter
		GLFW_KEY_P,	// togle emiter matrix update
		GLFW_KEY_EQUAL,	// increase emiter size
		GLFW_KEY_MINUS,	// decrease emiter size
		GLFW_KEY_O,	// emiter velocity ++
		GLFW_KEY_L	// emiter velocity --
	});

	emiter = EmiterManager::createEmiter(this, 2, 1000.0f, 2);
}


void SimpleCameraController::handleMouseMove(float xoffset, float yoffset)
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
		switch (key) {
		case GLFW_KEY_0:
			emiter->toggleActive();
			break;
		case GLFW_KEY_P:
			emiter->togleMatrixUpdate();
			break;
		case GLFW_KEY_EQUAL:
			emiter->increaseSize(1);
			break;
		case GLFW_KEY_MINUS:
			emiter->decreaseSize(1);
			break;
		case GLFW_KEY_O:
			emiter->increaseVelocity();
			break;
		case GLFW_KEY_L:
			emiter->decreaseVelocity();
			break;
		default:
			break;
		}
	}
}

const std::vector<const Scene::Camera*> SimpleCameraController::provideCameras() const
{
	return std::vector<const Scene::Camera*>{ &this->camera };
}

glm::mat4 SimpleCameraController::getEmiterMatrix() const
{
	const glm::vec3 positionOffset = camera.getFront() * 20.0f - camera.getUp() * 2.0f;
	return glm::mat4(glm::vec4(camera.getRight(), 0), glm::vec4(camera.getUp(), 0), glm::vec4(camera.getFront(), 0), glm::vec4(camera.getPosition() + positionOffset, 1));
}
