#include "ParticleObjectManager.h"


void ParticleObjectManager::moveObject(int objectNumber, glm::vec3 targetPosition, glm::vec3 targetDirection)
{
	m_positionChanged = true;
	m_partObjectsArray[objectNumber].m_targetPosition = targetPosition;
	m_partObjectsArray[objectNumber].m_targetVector = targetDirection;
}

glm::vec3* ParticleObjectManager::getObjectsPositions()
{
	static glm::vec3 positions[Configuration.MAX_PARTICLE_OBJECTS];
	for (int i = 0; i < m_numOfObjects; i++) {
		positions[i] = m_partObjectsArray[i].m_currentPosition;
	}
	return positions;
}

glm::vec3* ParticleObjectManager::getObjectsDirections()
{
	static glm::vec3 directions[Configuration.MAX_PARTICLE_OBJECTS];
	for (int i = 0; i < m_numOfObjects; i++) {
		directions[i] = m_partObjectsArray[i].m_currentVector;
	}
	return directions;
}

void ParticleObjectManager::init()
{
}


void ParticleObjectManager::synchronizeWithGpu()
{
	ParticleData::openObjects();
	for (int i = 0; i < m_numOfObjects; i++) {
		if (m_partObjectsArray[i].m_indEnd != ParticleData::m_resObjectsArray[i].m_indEnd) {
			// this object is only on CPU not on GPU -> copy all data
			ParticleData::m_resObjectsArray[i] = m_partObjectsArray[i];
		}
		else {
			// copy only target position and vector
			ParticleData::m_resObjectsArray[i].m_targetPosition = m_partObjectsArray[i].m_targetPosition;
			ParticleData::m_resObjectsArray[i].m_targetVector = m_partObjectsArray[i].m_targetVector;

			// get current position and vector
			m_partObjectsArray[i].m_currentPosition = ParticleData::m_resObjectsArray[i].m_currentPosition;
			m_partObjectsArray[i].m_currentVector = ParticleData::m_resObjectsArray[i].m_currentVector;
		}
	}
	ParticleData::commitObjects();
}

int ParticleObjectManager::addObject(const ParticleObject& object)
{
	m_partObjectsArray[m_numOfObjects] = object;

	m_numOfObjects++;
	return m_numOfObjects;
}

void ParticleObjectManager::printObjects(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "LOCAL Particle Objects print");
	
	LOG_F(INFO, "\tNum of objects: %d", m_numOfObjects);

	for (int i = 0; i < m_numOfObjects; i++) {
		LOG_F(INFO, "%d:\t%s", i, m_partObjectsArray[i].print().c_str());
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
