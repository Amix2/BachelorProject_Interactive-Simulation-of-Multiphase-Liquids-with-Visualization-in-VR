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

void ParticleData::openFluidArray()
{
	LOG_F(INFO, "OPEN to add array for FLUID");
	m_resFluidArray = (float*)GpuResources::openPartSSBO(BufferDatails.particlePositionsName
		, (GLintptr)m_FluidParticlesNum * 3 * sizeof(float)	// offset
		, ((GLsizeiptr)Configuration.MAX_FLUID_PARTICLES - m_FluidParticlesNum) * 3 * sizeof(float)	// length
	);
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openGlassArray()
{
	LOG_F(INFO, "OPEN to add array for GLASS");
	m_resGlassArray = (float*)GpuResources::openPartSSBO(BufferDatails.glassPositionsName
		, (GLintptr) m_GlassParticlesNum * 3 * sizeof(float)	// offset
		, ((GLsizeiptr) Configuration.MAX_GLASS_PARTICLES - m_GlassParticlesNum) * 3 * sizeof(float)	// length
	);
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openDetails()
{
	LOG_F(INFO, "OPEN details struct");
	m_resDetails = (SimDetails*)GpuResources::openSSBO(BufferDatails.detailsName);
	ParticleData::m_ResourceCondVariable.notify_all();
}


void ParticleData::commitFluidArray()
{

	LOG_F(INFO, "COMMIT fluid %d particles, we had %d fluid & %d glas", (int)m_numOfAddedFluid, m_FluidParticlesNum, m_GlassParticlesNum);

	m_FluidParticlesNum += m_numOfAddedFluid;
	m_numOfAddedFluid = 0;

	GpuResources::commitSSBO(BufferDatails.particlePositionsName);

	m_resFluidArray = nullptr;

	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitGlassArray()
{

	LOG_F(INFO, "COMMIT glass %d particles, we had %d fluid & %d glass", (int)m_numOfAddedGlass, m_FluidParticlesNum, m_GlassParticlesNum);

	m_GlassParticlesNum += m_numOfAddedGlass;

	GpuResources::commitSSBO(BufferDatails.glassPositionsName);

	m_resGlassArray = nullptr;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitDetails()
{
	LOG_F(INFO, "COMMIT details");

	GpuResources::commitSSBO(BufferDatails.detailsName);

	m_resDetails = nullptr;
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

	LOG_F(INFO, "\tNum of particles in simulation: %d, on CPU side: %d", details->numOfParticles, m_FluidParticlesNum);
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < max(details->numOfParticles, m_FluidParticlesNum) && i < limit; i++) {
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

	LOG_F(INFO, "\tNum of glass particles in simulation: %d, on CPU side: %d", details->numOfGlassParticles, m_GlassParticlesNum);
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < max(details->numOfGlassParticles, m_GlassParticlesNum) && i < limit; i++) {
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