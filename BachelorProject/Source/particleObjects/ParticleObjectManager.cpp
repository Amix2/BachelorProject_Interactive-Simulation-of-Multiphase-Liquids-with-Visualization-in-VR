#include "ParticleObject.h"

void ParticleObjectManager::init()
{
	m_partObjectsVec.reserve(5);
}

int ParticleObjectManager::addObject(const ParticleObject& object)
{
	m_partObjectsVec.push_back(object);
	m_numOfObjects++;
	return 0;
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
	int n = 0;
	for (auto it = m_partObjectsVec.begin(); it != m_partObjectsVec.end(); it++) {
		LOG_F(INFO, "%d:\t%s", n, it->print().c_str());
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
