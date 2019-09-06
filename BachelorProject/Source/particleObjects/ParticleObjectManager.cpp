#include "ParticleObjectManager.h"


void ParticleObjectManager::moveObject(int objectNumber, glm::vec3 targetPosition, glm::vec3 targetDirection)
{
	m_positionChanged = true;
	m_partObjectsArray[objectNumber].m_targetPosition = targetPosition;
	m_partObjectsArray[objectNumber].m_targetVector = targetDirection;
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
