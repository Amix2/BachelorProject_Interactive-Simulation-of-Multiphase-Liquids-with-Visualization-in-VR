#include "EmittingCameraController.h"

EmittingCameraController::EmittingCameraController(InputDispatcher& inputDispatcher, ViewPort& viewport, const glm::vec3& position)
	: SimpleCameraController{ inputDispatcher , viewport, position }
{
	inputDispatcher.subscribeForMousePositionChanges(this);
	inputDispatcher.subscribeForMouseScroll(this);
	inputDispatcher.subscribeForKeyInput(this, std::vector<int>{
		GLFW_KEY_0,	// togle emiter
			GLFW_KEY_P,	// togle emiter matrix update
			GLFW_KEY_EQUAL,	// increase emiter size
			GLFW_KEY_MINUS,	// decrease emiter size
			GLFW_KEY_O,	// emiter velocity ++
			GLFW_KEY_L	// emiter velocity --
	});

	emiter = EmiterManager::createEmiter(2, 1000.0f, 2, false);
	emiter->setRender(!m_updateEmiterMatrix);
}


void EmittingCameraController::handleMouseMove(float xoffset, float yoffset)
{
	SimpleCameraController::handleMouseMove(xoffset, yoffset);
	emiter->updateMatrix(this->getEmiterMatrix());

}

void EmittingCameraController::handleMouseScroll(float scroll)
{
	SimpleCameraController::handleMouseScroll(scroll);
}


void EmittingCameraController::handleKeyPress(int key, KeyState action, float deltaTime)
{
	SimpleCameraController::handleKeyPress(key, action, deltaTime);

	emiter->updateMatrix(this->getEmiterMatrix());

	if (action == KeyState::FALLING_EDGE) {
		switch (key) {
		case GLFW_KEY_0:
			emiter->toggleActive();
			break;
		case GLFW_KEY_P:
			m_updateEmiterMatrix = !m_updateEmiterMatrix;
			emiter->setMatrixUpdate(m_updateEmiterMatrix);
			emiter->setRender(!m_updateEmiterMatrix);
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


glm::mat4 EmittingCameraController::getEmiterMatrix() const
{
	const glm::vec3 positionOffset = camera.getFront() * 20.0f - camera.getUp() * 2.0f;
	return glm::mat4(glm::vec4(camera.getRight(), 0), glm::vec4(camera.getUp(), 0), glm::vec4(camera.getFront(), 0), glm::vec4(camera.getPosition() + positionOffset, 1));
}


const std::vector<const Scene::Camera*> EmittingCameraController::provideCameras() const
{
	return SimpleCameraController::provideCameras();
}
