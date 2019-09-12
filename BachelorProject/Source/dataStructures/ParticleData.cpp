#include "ParticleData.h"

//ParticleObject* ParticleData::m_resObjectsArray = (ParticleObject*)nullptr;

void ParticleData::initArraysOnGPU()
{
	// create SSBO for particle positions
	GpuResources::createSSBO(BufferDatails.particlePositionsName, 3 * Configuration.MAX_FLUID_PARTICLES * sizeof(float), NULL, BufferDatails.particlePositionsBinding);

	// create SSBO for particle fluid types
	GpuResources::createSSBO(BufferDatails.particlesFluidTypeName, Configuration.MAX_FLUID_PARTICLES * sizeof(int), NULL, BufferDatails.particlesFluidTypeBinding);

	// create SSBO for glass positions
	GpuResources::createSSBO(BufferDatails.glassPositionsName, 3 * Configuration.MAX_GLASS_PARTICLES * sizeof(float), NULL, BufferDatails.glassPositionsBinding);

	// create UBO for glass vectors
	GpuResources::createSSBO(BufferDatails.glassVectorName, 3 * Configuration.MAX_GLASS_PARTICLES * sizeof(float), NULL, BufferDatails.glassVectorBinding);

	// create SSBO for particle objects
	ParticleObject objectsArray[Configuration.MAX_PARTICLE_OBJECTS];
	GpuResources::createSSBO(BufferDatails.partObjectsName, Configuration.MAX_PARTICLE_OBJECTS * sizeof(ParticleObject), objectsArray, BufferDatails.partObjectsBinding);

	// create SSBO for simulation details
	SimDetails simDetails{ 0,0 };
	GpuResources::createSSBO(BufferDatails.detailsName, sizeof(simDetails), &simDetails, BufferDatails.detailsBinding);

	GpuResources::createSSBO(BufferDatails.SPHVariablesName, (GLsizeiptr)Configuration.MAX_FLUID_PARTICLES * Configuration.NUM_OF_SPH_FLOATS_PER_PARTICLE * sizeof(float), NULL, BufferDatails.SPHVariablesBinding);

	// create UBO with this data
	GpuResources::createUBO(BufferDatails.fluidTypesName, Configuration.MAX_FLUID_TYPES * sizeof(FluidType), FluidType::m_fluidTypes, BufferDatails.fluidTypesBinding);

	checkOpenGLErrors();
}

void ParticleData::openFluidArray()
{
	LOG_F(INFO, "OPEN to add array for FLUID");
	m_resFluidArray = (float*)GpuResources::openPartSSBO(BufferDatails.particlePositionsName
		, (GLintptr)m_FluidParticlesNum * 3 * sizeof(float)	// offset
		, ((GLsizeiptr)Configuration.MAX_FLUID_PARTICLES - m_FluidParticlesNum) * 3 * sizeof(float)	// length
	);
	m_resFluidTypesArray = (int*)GpuResources::openPartSSBO(BufferDatails.particlesFluidTypeName
		, (GLintptr)m_FluidParticlesNum  * sizeof(int)	// offset
		, ((GLsizeiptr)Configuration.MAX_FLUID_PARTICLES - m_FluidParticlesNum) * sizeof(int)	// length
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

void ParticleData::openGlassVectors()
{
	LOG_F(INFO, "OPEN to add array for GLASS VECTOR");
	m_resGlassVectorsArray = (float*)GpuResources::openPartSSBO(BufferDatails.glassVectorName
		, (GLintptr)m_GlassParticlesNum * 3 * sizeof(float)	// offset
		, ((GLsizeiptr)Configuration.MAX_GLASS_PARTICLES - m_GlassParticlesNum) * 3 * sizeof(float)	// length
	);
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openDetails()
{
	LOG_F(INFO, "OPEN details struct");
	m_resDetails = (SimDetails*)GpuResources::openSSBO(BufferDatails.detailsName);
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openObjects()
{
	LOG_F(INFO, "OPEN Particle Objects Array");
	m_resObjectsArray = (ParticleObject*)GpuResources::openSSBO(BufferDatails.partObjectsName);
	ParticleData::m_ResourceCondVariable.notify_all();
}


void ParticleData::commitFluidArray()
{

	LOG_F(INFO, "COMMIT fluid %d particles, we had %d fluid & %d glass", (int)m_numOfAddedFluid, m_FluidParticlesNum, m_GlassParticlesNum);

	m_FluidParticlesNum += m_numOfAddedFluid;
	m_numOfAddedFluid = 0;

	GpuResources::commitSSBO(BufferDatails.particlePositionsName);
	GpuResources::commitSSBO(BufferDatails.particlesFluidTypeName);

	m_resFluidArray = nullptr;
	m_resFluidTypesArray = nullptr;
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

void ParticleData::commitGlassVectors()
{
	LOG_F(INFO, "COMMIT glass vectors %d particles, we had %d fluid & %d glass", (int)m_numOfAddedGlass, m_FluidParticlesNum, m_GlassParticlesNum);

	GpuResources::commitSSBO(BufferDatails.glassVectorName);

	m_resGlassVectorsArray = nullptr;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitDetails()
{
	LOG_F(INFO, "COMMIT details");

	GpuResources::commitSSBO(BufferDatails.detailsName);

	m_resDetails = nullptr;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitObjects()
{
	LOG_F(INFO, "COMMIT Particle Objects Array");

	GpuResources::commitSSBO(BufferDatails.partObjectsName);

	m_resObjectsArray = nullptr;
	ParticleData::m_ResourceCondVariable.notify_all();
}

/////////////////////////////////////////////////////////////////////////////////////////////



void ParticleData::printParticleData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation particles print");
	SimDetails* details = getDetails();
	float * partPositions = (float*)ParticleData::getPositions();

	if (details == nullptr || partPositions == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation fluid particles");
		return;
	}

	if (details == nullptr || partPositions == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation particles");
		return;
	}

	if (limit <= 0) {
		limit = INT_MAX;
	}

	LOG_F(INFO, "\tNum of particles in simulation: %d, on CPU side: %d", details->numOfParticles, m_FluidParticlesNum);
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && (i < std::min((int)details->numOfParticles, m_FluidParticlesNum) && i < limit); i++) {
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
	float* glassVectors = ParticleData::getGlassVectors();

	if (details == nullptr || partPositions == nullptr || glassVectors == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation glass particles");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}

	LOG_F(INFO, "\tNum of glass particles in simulation: %d, on CPU side: %d", details->numOfGlassParticles, m_GlassParticlesNum);
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < std::max((int)details->numOfGlassParticles, m_GlassParticlesNum) && i < limit; i++) {
		LOG_F(INFO, "\tGlass %d: \t( %.4f  %.4f  %.4f ) v: [ %.4f  %.4f  %.4f ]"
			, i, partPositions[3 * i], partPositions[3 * i + 1], partPositions[3 * i + 2], glassVectors[3 * i], glassVectors[3 * i + 1], glassVectors[3 * i + 2]);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
		
void ParticleData::printParticleObjectsData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation Particle Objects print");
	ParticleObject* partObjects = ParticleData::getParticleObjects();
	if (partObjects == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation Particle Objects");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}

	for (int i = 0; i < Configuration.MAX_PARTICLE_OBJECTS && i < limit; i++) {
		LOG_F(INFO, "\tObject %d: %s", i, partObjects[i].print().c_str());
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::logParticlePositions()
{
	if (ParticleData::m_resGlassArray != nullptr || ParticleData::m_resFluidArray != nullptr) {
		return;
	}
	SimDetails* details;
	if (ParticleData::m_resDetails == nullptr) {
		details = ParticleData::getDetails();
	}
	else {
		details = ParticleData::m_resDetails;
	}
	const int numOfFluidParts = details->numOfParticles;
	const int numOfGlassParts = details->numOfGlassParticles;
	float* partPositions;
	partPositions = (float*)ParticleData::getPositions();

	if (numOfFluidParts > Configuration.MAX_FLUID_PARTICLES) return;
	if (numOfGlassParts > Configuration.MAX_GLASS_PARTICLES) return;

	for (int i = 0; i < numOfFluidParts; i++) {
		partFile << partPositions[3 * i] << " " << partPositions[3 * i + 1] << " " << partPositions[3 * i + 2] << " " << 1 << " ";
	}

	float* glassPositions;
	glassPositions = (float*)ParticleData::getGlassPositions();
	for (int i = 0; i < numOfGlassParts; i++) {
		partFile << glassPositions[3 * i] << " " << glassPositions[3 * i + 1] << " " << glassPositions[3 * i + 2] << " " << 0 << " ";
	}
	partFile << "|";
}

////////////////////////////////////////////////////////
//	private getters

float* ParticleData::getPositions()
{
	return (float*)GpuResources::getDataSSBO(BufferDatails.particlePositionsName);
}

float* ParticleData::getGlassPositions()
{
	return (float*)GpuResources::getDataSSBO(BufferDatails.glassPositionsName);
}

float* ParticleData::getGlassVectors()
{
	return (float*)GpuResources::getDataSSBO(BufferDatails.glassVectorName);;
}

ParticleObject* ParticleData::getParticleObjects()
{
	return (ParticleObject*) GpuResources::getDataSSBO(BufferDatails.partObjectsName);
}

SimDetails* ParticleData::getDetails()
{
	return (SimDetails*)GpuResources::getDataSSBO(BufferDatails.detailsName);
}