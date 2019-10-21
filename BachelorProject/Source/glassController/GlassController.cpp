#include "GlassController.h"

GlassController::GlassController(InputDispatcher& inputDispatcher, const Scene::Camera& camera, const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram)
	: camera{ camera }
	, shaderProgram{ shaderProgram }
	, selectedProgram{ selectedProgram }
{
	inputDispatcher.subscribeForKeyInput(this, std::vector<int>{
		GLFW_KEY_5,
		GLFW_KEY_6,
		GLFW_KEY_7,
		GLFW_KEY_8,
		GLFW_MOUSE_BUTTON_LEFT,
	});
}

void GlassController::assignUntrackedObjects(Scene::Scene& scene)
{
	if(trackedObjects < ParticleObjectManager::m_numOfObjects)
		for (int i = trackedObjects; i < ParticleObjectManager::m_numOfObjects; i++) {
			GlassObject* object = new GlassObject{ shaderProgram, selectedProgram, *ParticleObjectManager::m_partObjectsVector[i] };
			glassObjects.push_back(object);
			scene.addMaterialObject(object);
			trackedObjects++;
		}
}

void GlassController::handleKeyPress(int key, KeyState action, float deltaTime)
{
	if (key == GLFW_MOUSE_BUTTON_LEFT && action == FALLING_EDGE) {
		float distanceFromCamera = -1.0f;
		int selectedParticleObjectIndex = -1;
		for (int i = 0; i < ParticleObjectManager::m_numOfObjects; i++) {
			const ParticleObject* currentParticleObject = ParticleObjectManager::m_partObjectsVector[i].get();
			glm::vec3 centerAsSeenFromCamera = camera.getProjectionMatrix() * camera.getViewMatrix() * glm::vec4(currentParticleObject->m_center, 1.0f);
			float distanceFromZAxis = glm::length(glm::vec2{ centerAsSeenFromCamera.x, centerAsSeenFromCamera.y });
			if (distanceFromZAxis < currentParticleObject->m_innerRadius)
				if (glm::length(centerAsSeenFromCamera) < distanceFromCamera || distanceFromCamera < 0.0f)
					selectedParticleObjectIndex = i;
		}

		if (selectedParticleObjectIndex >= 0) {
			if (currentlySelected == selectedParticleObjectIndex) {
				glassObjects[selectedParticleObjectIndex]->select(false);
				currentlySelected = -1;
			}
			else {
				glassObjects[selectedParticleObjectIndex]->select(true);

				if (currentlySelected != -1) {
					glassObjects[currentlySelected]->select(false);
				}
				currentlySelected = selectedParticleObjectIndex;
			}
		}
		else if (currentlySelected != -1) {
			glassObjects[currentlySelected]->select(false);
			currentlySelected = -1;
		}
	}
	else if (action == PRESSED) {
		switch (key)
		{
		case GLFW_KEY_5:
			if (currentlySelected != -1)
				ParticleObjectManager::moveObject(currentlySelected, true);
			break;
		case GLFW_KEY_6:
			if (currentlySelected != -1)
				ParticleObjectManager::moveObject(currentlySelected, false);
			break;
		case GLFW_KEY_7:
			if (currentlySelected != -1)
				ParticleObjectManager::m_partObjectsVector[currentlySelected]->m_matrix = glm::rotate(ParticleObjectManager::m_partObjectsVector[currentlySelected]->m_matrix, 0.001f, glm::vec3(1, 0, 0));
			break;
		case GLFW_KEY_8:
			if (currentlySelected != -1)
				ParticleObjectManager::m_partObjectsVector[currentlySelected]->m_matrix = glm::rotate(ParticleObjectManager::m_partObjectsVector[currentlySelected]->m_matrix, -0.001f, glm::vec3(1, 0, 0));
			break;
		default:
			break;
		}
	}
}
