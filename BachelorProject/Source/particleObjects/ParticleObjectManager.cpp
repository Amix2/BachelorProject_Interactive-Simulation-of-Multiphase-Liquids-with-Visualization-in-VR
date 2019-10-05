#include "ParticleObjectManager.h"


void ParticleObjectManager::moveObject(int objectNumber, glm::vec3 targetPosition, glm::vec3 targetDirection)
{
	//m_positionChanged = true;
	//m_partObjectsArray[objectNumber].m_targetPosition = targetPosition;
	//m_partObjectsArray[objectNumber].m_targetVector = targetDirection;
}

glm::vec3* ParticleObjectManager::getObjectsPositions()
{
	//static glm::vec3 positions[Configuration.MAX_PARTICLE_OBJECTS];
	//for (int i = 0; i < m_numOfObjects; i++) {
	//	positions[i] = m_partObjectsArray[i].m_currentPosition;
	//}
	//return positions;
	return NULL;
}

glm::vec3* ParticleObjectManager::getObjectsDirections()
{
	//static glm::vec3 directions[Configuration.MAX_PARTICLE_OBJECTS];
	//for (int i = 0; i < m_numOfObjects; i++) {
	//	directions[i] = m_partObjectsArray[i].m_currentVector;
	//}
	//return directions;
	return NULL;
}

void ParticleObjectManager::init()
{
}


void ParticleObjectManager::synchronizeWithGpu()
{
	if (!ParticleObjectManager::m_positionChanged) return;
	//LOG_F(INFO, "SYNC glass OBJECTS");
	ParticleData::openGlassObjects();
	for (int i = 0; i < m_numOfObjects; i++) {
		ParticleData::m_resGlassObjectsArray[i].matrix = m_partObjectsArray[i].m_matrix;
	}
	ParticleData::commitGlassObjects(0);
}

int ParticleObjectManager::addObject(const ParticleObject& object)
{
	m_partObjectsArray[m_numOfObjects] = object;

	m_numOfObjects++;
	ParticleObjectManager::m_positionChanged = true;
	return m_numOfObjects;
}

void ParticleObjectManager::printObjects(int limit)
{
	//LOG_F(INFO, "==============================");
	//LOG_F(INFO, "LOCAL Particle Objects print");
	//
	//LOG_F(INFO, "\tNum of objects: %d", m_numOfObjects);

	//for (int i = 0; i < m_numOfObjects; i++) {
	//	LOG_F(INFO, "%d:\t%s", i, m_partObjectsArray[i].print().c_str());
	//}

	//LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
