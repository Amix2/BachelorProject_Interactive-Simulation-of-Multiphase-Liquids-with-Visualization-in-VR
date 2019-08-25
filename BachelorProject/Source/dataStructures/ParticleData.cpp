#include "ParticleData.h"


void ParticleData::initArraysOnGPU()
{
	// create SSBO for positions
	//float positions[3*Configuration.MAX_FLUID_PARTICLES];
	GpuResources::createSSBO(BufferDatails.particlePositionsName, 3 * Configuration.MAX_FLUID_PARTICLES * sizeof(float), NULL, BufferDatails.particlePositionsBinding);

	// create SSBO for glass positions
	//float glassPositions[3 * Configuration.MAX_GLASS_PARTICLES];
	GpuResources::createSSBO(BufferDatails.glassPositionsName, 3 * Configuration.MAX_GLASS_PARTICLES * sizeof(float), NULL, BufferDatails.glassPositionsBinding);


	// create SSBO for simulation details
	SimDetails simDetails{ 0,0 };
	GpuResources::createSSBO(BufferDatails.detailsName, sizeof(simDetails), &simDetails, BufferDatails.detailsBinding);

	checkOpenGLErrors();
}

void ParticleData::openToAddFluidArray()
{
	LOG_F(INFO, "Open to add array for FLUID");
	m_ResourceArray = (float*)GpuResources::openPartSSBO(BufferDatails.particlePositionsName, m_FluidParticlesNum * 3 * sizeof(float), (Configuration.MAX_FLUID_PARTICLES - m_FluidParticlesNum) * 3 * sizeof(float));
	m_OpenResource = FLUID;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openToAddGlassArray()
{
	LOG_F(INFO, "Open to add array for GLASS");
	m_ResourceArray = (float*)GpuResources::openSSBO(BufferDatails.glassPositionsName);
	m_OpenResource = GLASS;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitToAddArray()
{
	GpuResources::commitSSBO(m_OpenResource == FLUID ? BufferDatails.particlePositionsName : BufferDatails.glassPositionsName);

	SimDetails* details = (SimDetails * )GpuResources::openSSBO(BufferDatails.detailsName);
	if (m_OpenResource == FLUID) {
		LOG_F(INFO, "Commit fluid %d particles, we had %d fluid & %d glas", m_AddedParticlesNum, m_FluidParticlesNum, m_GlassParticlesNum);
		details->numOfParticles += m_AddedParticlesNum;
		m_FluidParticlesNum += m_AddedParticlesNum;
	}
	else if (m_OpenResource == GLASS) {
		LOG_F(INFO, "Commit glass %d particles, we had %d fluid & %d glass", m_AddedParticlesNum, m_FluidParticlesNum, m_GlassParticlesNum);
		details->numOfGlassParticles += m_AddedParticlesNum;
		m_GlassParticlesNum += m_AddedParticlesNum;
	}
	else {
		throw "magic";
	}
	GpuResources::commitSSBO(BufferDatails.detailsName);
	m_ResourceArray = nullptr;
	m_OpenResource = NONE;
	ParticleData::m_ResourceCondVariable.notify_all();
}

/////////////////////////////////////////////////////////////////////////////////////////////


void ParticleData::printParticleData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation particles print");
	SimDetails* details = getDetails();
	float * partPositions = (float*)ParticleData::getPositions();

	if (limit <= 0) {
		limit = INT_MAX;
	}

	LOG_F(INFO, "\tNum of particles in simulation: %d", details->numOfParticles);
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < details->numOfParticles && i < limit; i++) {
		LOG_F(INFO, "\tParticle %d:\t( %.4f  %.4f  %.4f )", i, partPositions[3 * i], partPositions[3 * i + 1], partPositions[3 * i + 2]);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::printGlassData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation glass particles print");
	SimDetails* details = getDetails();
	float* partPositions = (float*)ParticleData::getGlassPositions();

	if (limit <= 0) {
		limit = INT_MAX;
	}

	LOG_F(INFO, "\tNum of glass particles in simulation: %d", details->numOfGlassParticles);
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < details->numOfGlassParticles && i < limit; i++) {
		LOG_F(INFO, "\tGlass %d: \t( %.4f  %.4f  %.4f )", i, partPositions[3 * i], partPositions[3 * i + 1], partPositions[3 * i + 2]);
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
	LOG_F(INFO, "\tTo-add particles in sim: num: %d, type: %d", numOfPart, partType);

	for (int i = 0; i < Configuration.MAX_PARTICLES_ADDED_IN_TURN && i < numOfPart && i < limit; i++) {
		LOG_F(INFO, "\tTo-add Part %d:\t( %.4f  %.4f  %.4f )", i, partPositions[3 * i +2], partPositions[3 * i + 1 + 2], partPositions[3 * i + 2 + 2]);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::logParticlePositions()
{
	SimDetails* details = getDetails();
	float * partPositions = (float*)ParticleData::getPositions();
	for (int i = 0; i < details->numOfParticles; i++) {
		partFile << partPositions[3 * i] << " " << partPositions[3 * i + 1] << " " << partPositions[3 * i + 2] << " " << 1 << " ";
	}
	float* glassPositions = (float*)ParticleData::getGlassPositions();
	for (int i = 0; i < details->numOfGlassParticles; i++) {
		partFile << glassPositions[3 * i] << " " << glassPositions[3 * i + 1] << " " << glassPositions[3 * i + 2] << " " << 0 << " ";
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