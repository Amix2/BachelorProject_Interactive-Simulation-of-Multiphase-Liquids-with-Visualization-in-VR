#include "ParticleObjectManager.h"


void ParticleObjectManager::init()
{
	LOG_F(INFO, "Initialize ParticleObjectManager");
	ParticleObjectManager::m_workerThread = std::thread(ParticleObjectManager::runWorkerThread);
	Threads::addThreadToList(&m_workerThread);
}

void ParticleObjectManager::runWorkerThread()
{
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
	LOG_F(INFO, "Simulation particle objects print");
	
	LOG_F(INFO, "\tNum of objects: %d", m_numOfObjects);

	for (int i = 0; i < m_numOfObjects; i++) {
		LOG_F(INFO, "%d:\t%s", i, m_partObjectsArray[i].print().c_str());
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
