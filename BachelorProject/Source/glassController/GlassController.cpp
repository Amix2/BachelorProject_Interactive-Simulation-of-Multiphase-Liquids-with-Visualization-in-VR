#include "GlassController.h"

GlassController::GlassController(InputDispatcher& inputDispatcher, const Scene::Camera& camera, const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram)
	: camera{ camera }
	, shaderProgram{ shaderProgram }
	, selectedProgram{ selectedProgram }
{
	inputDispatcher.subscribeForKeyInput(this, std::vector<int>{
		GLFW_KEY_3,
		GLFW_KEY_4,
		GLFW_KEY_5,
		GLFW_KEY_6,
		GLFW_KEY_7,
		GLFW_KEY_8,
		GLFW_MOUSE_BUTTON_LEFT,
		GLFW_MOUSE_BUTTON_RIGHT
	});

	inputDispatcher.subscribeForMousePositionChanges(this);
}

void GlassController::assignUntrackedObjects(Scene::Scene& scene)
{
		for (; trackedObjects < ParticleObjectManager::m_numOfObjects; trackedObjects++) {
			ParticleObjectManager::m_partObjectsVector[trackedObjects]->createGlassObject(shaderProgram, selectedProgram);
			scene.addMaterialObject(ParticleObjectManager::m_partObjectsVector[trackedObjects].get(), 0);
		}
}

void GlassController::handleKeyPress(int key, KeyState action, float deltaTime)
{
	if (action == FALLING_EDGE) {
		switch (key) {
		case GLFW_MOUSE_BUTTON_LEFT:
			selectGlass();
			moveAccumulator = 0.0f;
			destinationValid = false;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (currentlySelected != -1) {
				glassObjects[currentlySelected]->select(false);
				currentlySelected = -1;
			}
			break;
		case GLFW_KEY_3:
			for (auto& glassObject : ParticleObjectManager::m_partObjectsVector)
				glassObject->toggleRender();
			break;
		case GLFW_KEY_4:
			mode = MOVE;
			break;
		case GLFW_KEY_5:
			mode = ROTATE;
			break;
		case GLFW_KEY_6:
			selectedAxis = glm::vec3{ 1.0f, 0.0f, 0.0f };
			break;
		case GLFW_KEY_7:
			selectedAxis = glm::vec3{ 0.0f, 1.0f, 0.0f };
			break;
		case GLFW_KEY_8:
			selectedAxis = glm::vec3{ 0.0f, 0.0f, 1.0f };
			break;
		default:
			break; 
		}
	}
	else if (action == RISING_EDGE) {
		switch (key)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			submitMove();
			destinationValid = true;
			break;
		default:
			break;
		}
	}
}

void GlassController::handleMouseMove(float x, float y)
{
	if(!destinationValid)
		moveAccumulator -= SENSITIVITY * x;
}

const glm::mat4* GlassController::getCurrentlySelectedModelMatrix() const
{
	if (currentlySelected != -1)
		return &ParticleObjectManager::m_partObjectsVector[currentlySelected].get()->m_matrix;
	return nullptr;
}

const glm::mat4* GlassController::getCurrentlySelectedDestinationMatrix() const
{
	if (currentlySelected != -1)
		return &ParticleObjectManager::m_partObjectsVector[currentlySelected].get()->m_destinationMatrix;
	return nullptr;
}

void GlassController::selectGlass() {
	float distanceFromCamera = -1.0f;
	int selectedParticleObjectIndex = -1;
	for (int i = 0; i < ParticleObjectManager::m_numOfObjects; i++) {
		const ParticleObject* currentParticleObject = ParticleObjectManager::m_partObjectsVector[i].get();
		glm::vec3 centerAsSeenFromCamera = camera.getViewMatrix() * glm::vec4(currentParticleObject->m_matrix[3][0], currentParticleObject->m_matrix[3][1], currentParticleObject->m_matrix[3][2], 1.0f);
		float distanceFromZAxis = glm::length(glm::vec2{ centerAsSeenFromCamera.x, centerAsSeenFromCamera.y });
		if (distanceFromZAxis < currentParticleObject->m_innerRadius)
			if (glm::length(centerAsSeenFromCamera) < distanceFromCamera || distanceFromCamera < 0.0f)
				selectedParticleObjectIndex = i;
	}

	if (selectedParticleObjectIndex >= 0) {
		if (currentlySelected != selectedParticleObjectIndex) {
			glassObjects[selectedParticleObjectIndex]->grab();

			if (currentlySelected != -1) {
				ParticleObjectManager::m_partObjectsVector[currentlySelected]->release();
			}
			currentlySelected = selectedParticleObjectIndex;
		}
	}
	else if (currentlySelected != -1) {
		ParticleObjectManager::m_partObjectsVector[currentlySelected]->release();
		currentlySelected = -1;
	}
}

void GlassController::submitMove()
{
	if(glm::abs(moveAccumulator) > 1.0 && currentlySelected != -1) // EPSILON
		if(mode == MOVE) 
			ParticleObjectManager::moveObject(currentlySelected, moveAccumulator, selectedAxis);
		else if(mode == ROTATE)
			ParticleObjectManager::m_partObjectsVector[currentlySelected]->m_destinationMatrix = glm::rotate(ParticleObjectManager::m_partObjectsVector[currentlySelected]->m_destinationMatrix, 0.1f * moveAccumulator, selectedAxis);
}
