#include "ParticleData.h"


void ParticleData::initArraysOnGPU()
{
	// create SSBO for positions
	float positions[Configuration.MAX_FLUID_PARTICLES];
	GpuResources::createSSBO(BufferDatails.particlePositionsName, sizeof(positions), positions, BufferDatails.particlePositionsBinding);

	// create SSBO for glass positions
	float glassPositions[Configuration.MAX_GLASS_PARTICLES];
	GpuResources::createSSBO(BufferDatails.glassPositionsName, sizeof(glassPositions), glassPositions, BufferDatails.glassPositionsBinding);

	// create SSBO for to-add particle positions
	float toAddParticlePositions[Configuration.MAX_PARTICLES_ADDED_IN_TURN+2]; // [0] -> newPartType | [1] -> numOfNewParticles
	for (int i = 2; i < Configuration.MAX_PARTICLES_ADDED_IN_TURN; i++) {
		toAddParticlePositions[i] = -1;
	}
	GpuResources::createSSBO(BufferDatails.toAddParticlePositionsName, sizeof(toAddParticlePositions), toAddParticlePositions, BufferDatails.toAddParticlePositionsBinding);

	// create SSBO for simulation details
	SimDetails simDetails{ 0,0 };
	GpuResources::createSSBO(BufferDatails.detailsName, sizeof(simDetails), &simDetails, BufferDatails.detailsBinding);

	checkOpenGLErrors();
}

void ParticleData::addParticle(const float v3_positions[], int particleType, int numOfToAddParticles)
{
	LOG_F(INFO, "%d particless added", numOfToAddParticles);

	// open SSBO buffer
	float * partPositions = (float*)GpuResources::openSSBO(BufferDatails.toAddParticlePositionsName);

	// copy data to SSBO buffer in local adsress space
	memcpy(partPositions,		&particleType,			sizeof(int));
	memcpy(partPositions + 1,	&numOfToAddParticles,	sizeof(int));
	memcpy(partPositions + 2,	v3_positions,			3 * numOfToAddParticles * sizeof(float));

	// commit new data to GPU.  it might not be there yet, it will be updated in next 
	GpuResources::commitSSBO(BufferDatails.toAddParticlePositionsName);

	m_numOfAllParticles += numOfToAddParticles;
}



/////////////////////////////////////////////////////////////////////////////////////////////


void ParticleData::printParticleData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation particles print");
	SimDetails* details = getDetails();
	float * partPositions = (float*)ParticleData::getPositions();

	LOG_F(INFO, "Num of particles in simulation: %d", details->numOfParticles);
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < details->numOfParticles && i < limit; i++) {
		LOG_F(INFO, "Particle %d:\t( %.4f  %.4f  %.4f )", i, partPositions[3 * i], partPositions[3 * i + 1], partPositions[3 * i + 2]);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
		
void ParticleData::printToAddParticleData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Particles to add print");
	float * partPositions = (float*)ParticleData::getToAddParticlePositions();
	int partType;
	memcpy(&partType, partPositions, sizeof(int));
	int numOfPart;
	memcpy(&numOfPart, partPositions+1, sizeof(int));
	numOfPart = 2;
	LOG_F(INFO, "To-add particles in sim: num: %d, type: %d", numOfPart, partType);

	for (int i = 0; i < Configuration.MAX_PARTICLES_ADDED_IN_TURN && i < numOfPart && i < limit; i++) {
		LOG_F(INFO, "To-add Part %d:\t( %.4f  %.4f  %.4f )", i, partPositions[3 * i +2], partPositions[3 * i + 1 + 2], partPositions[3 * i + 2 + 2]);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::logParticlePositions()
{
	SimDetails* details = getDetails();
	float * partPositions = (float*)ParticleData::getPositions();
	for (int i = 0; i < details->numOfParticles; i++) {
		partFile << partPositions[3 * i] << " " << partPositions[3 * i + 1] << " " << partPositions[3 * i + 2] << " ";
	}
	partFile << "| ";
}

////////////////////////////////////////////////////////
//	private getters

void * ParticleData::getPositions()
{
	return GpuResources::getDataSSBO(BufferDatails.particlePositionsName);
}

void * ParticleData::getGlassPositions()
{
	return GpuResources::getDataSSBO(BufferDatails.glassPositionsName);
}

void * ParticleData::getToAddParticlePositions()
{
	return GpuResources::getDataSSBO(BufferDatails.toAddParticlePositionsName);
}

SimDetails* ParticleData::getDetails()
{
	return (SimDetails*)GpuResources::getDataSSBO(BufferDatails.detailsName);
}