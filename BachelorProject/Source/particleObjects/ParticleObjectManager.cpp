#include "ParticleObjectManager.h"


void ParticleObjectManager::moveObject(int objectNumber, float val)
{
	m_partObjectsVector[objectNumber]->m_matrix = glm::translate(m_partObjectsVector[0]->m_matrix, glm::vec3(val, 0, 0));
	m_positionChanged = true;
	//m_partObjectsVector[objectNumber].m_targetPosition = targetPosition;
	//m_partObjectsVector[objectNumber].m_targetVector = targetDirection;
}

glm::vec3* ParticleObjectManager::getObjectsPositions()
{
	//static glm::vec3 positions[Configuration.MAX_PARTICLE_OBJECTS];
	//for (int i = 0; i < m_numOfObjects; i++) {
	//	positions[i] = m_partObjectsVector[i].m_currentPosition;
	//}
	//return positions;
	return NULL;
}

glm::vec3* ParticleObjectManager::getObjectsDirections()
{
	//static glm::vec3 directions[Configuration.MAX_PARTICLE_OBJECTS];
	//for (int i = 0; i < m_numOfObjects; i++) {
	//	directions[i] = m_partObjectsVector[i].m_currentVector;
	//}
	//return directions;
	return NULL;
}

void ParticleObjectManager::init()
{
}


void ParticleObjectManager::synchronizeWithGpu()
{
	for (int i = 0; i < m_numOfObjects; i++) {
		m_partObjectsVector[i]->stepTowardsDestination();
	}
	//if (!ParticleObjectManager::m_positionChanged) return;
	ParticleObjectManager::m_positionChanged = false;
	//for (int i = 0; i < m_numOfObjects; i++) {
	//	ParticleData::m_resGlassObjectsArray[i].matrix = m_partObjectsVector[i]->m_matrix;
	//}
	//ParticleData::sendGlassObjects(0);
	//LOG_F(INFO, "SYNC glass OBJECTS");
	ParticleData::openGlassObjects();
	for (int i = 0; i < m_numOfObjects; i++) {
		ParticleData::m_resGlassObjectsArray__MAP__[i].matrix = m_partObjectsVector[i]->m_matrix;
	}
	ParticleData::commitGlassObjects(0);
}

int ParticleObjectManager::addObject(const ParticleObject& object)
{
	if (m_numOfObjects >= Configuration.MAX_PARTICLE_OBJECTS) {
		LOG_F(ERROR, "Too many particle objects, cannot create a new one");
		return m_numOfObjects;
	}
	m_partObjectsVector.push_back(std::make_unique<ParticleObject>(object));

	m_numOfObjects++;
	ParticleObjectManager::m_positionChanged = true;
	LOG_F(WARNING, "New %s", object.toString().c_str());
	return m_numOfObjects;
}

int ParticleObjectManager::addObject(const MugParticleObject& object)
{
	if (m_numOfObjects >= Configuration.MAX_PARTICLE_OBJECTS) {
		LOG_F(ERROR, "Too many particle objects, cannot create a new one");
		return m_numOfObjects;
	}
	m_partObjectsVector.push_back(std::make_unique<MugParticleObject>(object));

	m_numOfObjects++;
	ParticleObjectManager::m_positionChanged = true;

	LOG_F(WARNING, "New %s", object.toString().c_str());
	return m_numOfObjects;
}

void ParticleObjectManager::printObjects(int limit)
{
	//LOG_F(INFO, "==============================");
	//LOG_F(INFO, "LOCAL Particle Objects print");
	//
	//LOG_F(INFO, "\tNum of objects: %d", m_numOfObjects);

	//for (int i = 0; i < m_numOfObjects; i++) {
	//	LOG_F(INFO, "%d:\t%s", i, m_partObjectsVector[i].print().c_str());
	//}

	//LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
