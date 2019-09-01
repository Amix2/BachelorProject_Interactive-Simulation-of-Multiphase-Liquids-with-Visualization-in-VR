#include "ParticleObjectManager.h"


int ParticleObjectManager::addObject(const ParticleObject& object)
{
	m_partObjectsArray[m_numOfObjects] = object;

	m_numOfObjects++;
	return m_numOfObjects;
}

int ParticleObjectManager::selectObject(glm::vec3 handPosition) {
	return -1;
}

void ParticleObjectManager::printObjects(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation particle objects print");
	
	LOG_F(INFO, "\tNum of objects: %d", m_numOfObjects);
	LOG_F(INFO, "\tCurrently selected: %d", m_selectedObjectIndex);
	for (int i = 0; i < m_numOfObjects; i++) {
		LOG_F(INFO, "%d:\t%s", i, m_partObjectsArray[i].print().c_str());
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
