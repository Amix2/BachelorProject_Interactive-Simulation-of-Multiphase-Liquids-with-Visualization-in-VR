#include "ParticleData.h"


int ParticleData::m_numOfParticles = 0;

void * ParticleData::getPositions()
{
	return GpuResources::getDataSSBO(BufferDatails.particlePositionsName);
}

void ParticleData::initArraysOnGPU()
{
	// create SSBO for positions
	float positions[Configuration.MAX_FLUID_PARTICLES];
	GpuResources::createSSBO(BufferDatails.particlePositionsName, sizeof(positions), positions, BufferDatails.particlePositionsBinding);
}

void ParticleData::addParticle(const float v3_positions[], int numOfParticlesAdded)
{
	LOG_F(INFO, "%d particless added", numOfParticlesAdded);

	const int ind = m_numOfParticles * 3;
	float * partPositions = (float*) ParticleData::getPositions();
	memcpy(&(partPositions[ind]), v3_positions, 3 * numOfParticlesAdded * sizeof(float));
	m_numOfParticles += numOfParticlesAdded;
}
									