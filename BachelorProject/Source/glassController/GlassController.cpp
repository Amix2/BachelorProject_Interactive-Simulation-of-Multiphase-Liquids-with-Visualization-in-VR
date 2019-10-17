#include "GlassController.h"

GlassController::GlassController(Window& window, const Scene::Camera& camera, const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram, const glm::vec4& background)
	: camera{ camera }
	, shaderProgram{ shaderProgram }
	, selectedProgram{ selectedProgram }
	, background{ background }
{
	window.subscribeForKeyInput(this);
}

void GlassController::assignUntrackedObjects(Scene::Scene& scene)
{
	if(trackedObjects < ParticleObjectManager::m_numOfObjects)
		for (int i = trackedObjects; i < ParticleObjectManager::m_numOfObjects; i++) {
			GlassObject* object = new GlassObject{ shaderProgram, selectedProgram, background, ParticleObjectManager::m_partObjectsArray[i] };
			glassObjects.push_back(object);
			scene.addMaterialObject(object);
			trackedObjects++;
		}
}

void GlassController::handleKeyPress(Key key)
{
	switch (key)
	{
	case MOUSE_LEFT:
	{
		float distanceFromCamera = -1.0f;
		int selectedParticleObjectIndex = -1;
		for (int i = 0; i < ParticleObjectManager::m_numOfObjects; i++) {
			const ParticleObject* currentParticleObject = &ParticleObjectManager::m_partObjectsArray[i];
			glm::vec3 centerAsSeenFromCamera = camera.getViewMatrix() * glm::vec4(currentParticleObject->m_center, 1.0f);
			float distanceFromZAxis = glm::length(glm::vec2{ centerAsSeenFromCamera.x, centerAsSeenFromCamera.y });
			if (distanceFromZAxis < currentParticleObject->m_innerRadius)
				if (glm::length(centerAsSeenFromCamera) < distanceFromCamera || distanceFromCamera < 0.0f)
					selectedParticleObjectIndex = 0;
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
		break;
	}
	case KEY_5:
		if(currentlySelected != -1)
			ParticleObjectManager::moveObject(currentlySelected, true);
		break;
	case KEY_6:
		if (currentlySelected != -1)
			ParticleObjectManager::moveObject(currentlySelected, false);
		break;
	case KEY_7:
		if (currentlySelected != -1)
			ParticleObjectManager::m_partObjectsArray[currentlySelected].m_matrix = glm::rotate(ParticleObjectManager::m_partObjectsArray[currentlySelected].m_matrix, 0.001f, glm::vec3(1, 0, 0));
		break;
	case KEY_8:
		if (currentlySelected != -1)
			ParticleObjectManager::m_partObjectsArray[currentlySelected].m_matrix = glm::rotate(ParticleObjectManager::m_partObjectsArray[currentlySelected].m_matrix, -0.001f, glm::vec3(1, 0, 0));
		break;
	default:
		break;
	}
}
