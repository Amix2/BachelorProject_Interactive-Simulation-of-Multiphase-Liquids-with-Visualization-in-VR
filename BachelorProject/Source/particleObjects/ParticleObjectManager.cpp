#include "ParticleObjectManager.h"


void ParticleObjectManager::moveObject(int objectNumber, float val, glm::vec3 axis)
{
	m_partObjectsVector[objectNumber]->m_destinationMatrix = glm::translate(m_partObjectsVector[objectNumber]->m_destinationMatrix, val * axis);
}


void ParticleObjectManager::init()
{
	m_partObjectsVector.reserve(Configuration.MAX_PARTICLE_OBJECTS);
}


void ParticleObjectManager::synchronizeWithGpu()
{
	for (int i = 0; i < m_numOfObjects; i++) {
		m_partObjectsVector[i]->stepTowardsDestination();
	}

	ParticleData::openGlassObjects();
	for (int i = 0; i < m_numOfObjects; i++) {
		ParticleData::m_resGlassObjectsArray__MAP__[i].matrix = m_partObjectsVector[i]->m_matrix;
	}
	ParticleData::commitGlassObjects(0);
}


int ParticleObjectManager::addObject(std::unique_ptr<MugParticleObject> object)
{
	if (m_numOfObjects >= Configuration.MAX_PARTICLE_OBJECTS) {
		LOG_F(ERROR, "Too many particle objects, cannot create a new one");
		return m_numOfObjects;
	}
	LOG_F(WARNING, "New %s", object->toString().c_str());
	m_partObjectsVector.push_back(std::move(object));
	m_numOfObjects++;

	SelectableObjectManager::addSelectableObject(m_partObjectsVector[m_partObjectsVector.size()-1].get());

	return m_numOfObjects;
}

void ParticleObjectManager::printObjects(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "LOCAL Particle Objects print");
	
	LOG_F(INFO, "\tNum of objects: %d", m_numOfObjects.load());

	for (int i = 0; i < m_numOfObjects; i++) {
		LOG_F(INFO, "%d:\t%s", i, m_partObjectsVector[i]->toString().c_str());
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
