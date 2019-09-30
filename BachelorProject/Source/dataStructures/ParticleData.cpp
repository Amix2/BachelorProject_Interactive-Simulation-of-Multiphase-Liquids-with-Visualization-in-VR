#include "ParticleData.h"

//ParticleObject* ParticleData::m_resObjectsArray = (ParticleObject*)nullptr;

void ParticleData::initArraysOnGPU()
{
	// create SSBO for particle positions
	GpuResources::createSSBO(BufferDatails.particlePositionsName, 4 * Configuration.MAX_FLUID_PARTICLES * sizeof(float), NULL, BufferDatails.particlePositionsBinding);

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

	// SSBO for sorting
	GpuResources::createSSBO(BufferDatails.SortVariablesName, Configuration.NUM_OF_SORTING_FLOATS_IN_ARRAY * sizeof(float), NULL, BufferDatails.SortVariablesBinding);

	// SSBO for neighbour list
	GpuResources::createSSBO(BufferDatails.NeighboursListName, (GLsizeiptr)27 * Configuration.MAX_FLUID_PARTICLES * sizeof(GLuint), NULL, BufferDatails.NeighboursListBinding);

	checkOpenGLErrors();
}

void ParticleData::openFluidArray()
{
	LOG_F(INFO, "OPEN to add array for FLUID");
	m_resFluidArray = (FluidParticle*)GpuResources::openPartSSBO(BufferDatails.particlePositionsName
		, (GLintptr)m_FluidParticlesNum * 4 * sizeof(float)	// offset
		, ((GLsizeiptr)Configuration.MAX_FLUID_PARTICLES - m_FluidParticlesNum) * 4 * sizeof(float)	// length
	);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
}


void ParticleData::openGlassArray()
{
	LOG_F(INFO, "OPEN to add array for GLASS");
	m_resGlassArray = (float*)GpuResources::openPartSSBO(BufferDatails.glassPositionsName
		, (GLintptr) m_GlassParticlesNum * 3 * sizeof(float)	// offset
		, ((GLsizeiptr) Configuration.MAX_GLASS_PARTICLES - m_GlassParticlesNum) * 3 * sizeof(float)	// length
	);
	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openGlassVectors()
{
	LOG_F(INFO, "OPEN to add array for GLASS VECTOR");
	m_resGlassVectorsArray = (float*)GpuResources::openPartSSBO(BufferDatails.glassVectorName
		, (GLintptr)m_GlassParticlesNum * 3 * sizeof(float)	// offset
		, ((GLsizeiptr)Configuration.MAX_GLASS_PARTICLES - m_GlassParticlesNum) * 3 * sizeof(float)	// length
	);
	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openDetails()
{
	LOG_F(INFO, "OPEN details struct");
	m_resDetails = (SimDetails*)GpuResources::openSSBO(BufferDatails.detailsName);
	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::openObjects()
{
	//LOG_F(INFO, "OPEN Particle Objects Array");
	m_resObjectsArray = (ParticleObject*)GpuResources::openSSBO(BufferDatails.partObjectsName);
	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
}


void ParticleData::commitFluidArray()
{

	LOG_F(INFO, "COMMIT fluid %d particles, we had %d fluid & %d glass", (int)m_numOfAddedFluid, m_FluidParticlesNum, m_GlassParticlesNum);

	m_FluidParticlesNum += m_numOfAddedFluid;
	m_numOfAddedFluid = 0;

	GpuResources::commitSSBO(BufferDatails.particlePositionsName);

	m_resFluidArray = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitGlassArray()
{
	LOG_F(INFO, "COMMIT glass %d particles, we had %d fluid & %d glass", (int)m_numOfAddedGlass, m_FluidParticlesNum, m_GlassParticlesNum);

	m_GlassParticlesNum += m_numOfAddedGlass;

	GpuResources::commitSSBO(BufferDatails.glassPositionsName);

	m_resGlassArray = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitGlassVectors()
{
	LOG_F(INFO, "COMMIT glass vectors %d particles, we had %d fluid & %d glass", (int)m_numOfAddedGlass, m_FluidParticlesNum, m_GlassParticlesNum);

	GpuResources::commitSSBO(BufferDatails.glassVectorName);

	m_resGlassVectorsArray = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitDetails()
{
	LOG_F(INFO, "COMMIT details");

	GpuResources::commitSSBO(BufferDatails.detailsName);

	m_resDetails = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitObjects()
{
	//LOG_F(INFO, "COMMIT Particle Objects Array");

	GpuResources::commitSSBO(BufferDatails.partObjectsName);

	m_resObjectsArray = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::copyDataForSorting()
{
	// check "layout(std430, binding = 9) buffer sortingHelpBuf" buffer for details
	/*
	uint sortIndexArray[SORT_ARRAY_SIZE];
	uint originalIndex[SORT_ARRAY_SIZE];
	float	CPY_Positions[3 * MAX_FLUID];
	float	CPY_Velocity[3 * MAX_FLUID];
	int		CPY_FluidTypes[MAX_FLUID];
	*/
	const int sortIdSize = 2*Configuration.SORT_ARRAY_SIZE * sizeof(float);
	const int cpyPosSize = 4 * Configuration.MAX_FLUID_PARTICLES * sizeof(float);
	const int cpyPosSizeUsed = 4 * ParticleData::m_FluidParticlesNum * sizeof(float);
	const int cpyVelSize = cpyPosSize;
	const int cpyVelSizeUsed = cpyPosSizeUsed;
	const std::string sortTargetName = BufferDatails.SortVariablesName;
	const std::string positionsSourceName = BufferDatails.particlePositionsName;
	const std::string velositySourceName = BufferDatails.SPHVariablesName;
	//const std::string typesSourceName = BufferDatails.particlesFluidTypeName;

	//glFinish();	auto start = getNanoTime();
	GpuResources::setAsCopyTarget(sortTargetName);
	GpuResources::setAsCopySource(positionsSourceName);
	GpuResources::copyResourceSubData(positionsSourceName, sortTargetName, 0, sortIdSize, cpyPosSizeUsed);
	GpuResources::setAsCopySource(velositySourceName);
	GpuResources::copyResourceSubData(velositySourceName, sortTargetName, 0, sortIdSize + cpyPosSize, cpyVelSizeUsed);
	//GpuResources::setAsCopySource(typesSourceName);
	//GpuResources::copyResourceSubData(typesSourceName, sortTargetName, 0, sortIdSize + cpyPosSize + cpyVelSize, cpyTypesSizeUsed);
	//glFinish();  auto end = getNanoTime();

	//LOG_F(ERROR, "TIIMEEE:   %f", getNanoTimeDif(start, end));
	checkOpenGLErrors();
}

/////////////////////////////////////////////////////////////////////////////////////////////



void ParticleData::printParticleData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation particles print");
	SimDetails* details = getDetails();
	FluidParticle* partPositions = ParticleData::getPositions();

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
		LOG_F(INFO, "\tParticle %d:\t( %.4f  %.4f  %.4f ) [%d]", i, partPositions[i].x, partPositions[i].y, partPositions[i].z, partPositions[i].type);
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
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < std::max((int)details->numOfGlassParticles, m_GlassParticlesNum) || i < limit; i++) {
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

void ParticleData::printSortingData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "SORTING print");
	unsigned int* array = ParticleData::getSortingData();
	if (array == nullptr) {
		LOG_F(ERROR, "Error while printing SORTING DATA");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}

	unsigned int prevValue = INT_MAX;
	for (int i = 0; i < Configuration.SORT_ARRAY_SIZE && i < limit; i++) {
		LOG_F(INFO, "\t %d: %u [%u]", i, array[i], array[Configuration.SORT_ARRAY_SIZE+i]);
	}
	bool sorted = true;
	for (int i = 0; i < Configuration.SORT_ARRAY_SIZE; i++) {
		if (array[i] > prevValue) sorted = false;
		prevValue = array[i];
	}
	LOG_F(INFO, "SORTED: %d", sorted);
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::printNeighboursData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "NEIGHBOURS print");
	int* array = ParticleData::getNeighboursData();
	if (array == nullptr) {
		LOG_F(ERROR, "Error while printing NEIGHBOURS DATA");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}

	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < limit && i < ParticleData::m_FluidParticlesNum; i++) {
		std::stringstream ss;
		for (int x = 0; x < 27; x++) {
			ss << array[27 * i + x] << ", ";
		}
		LOG_F(INFO, "\t %d: %s", i, ss.str().c_str());
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::printSPHData(bool velocity, bool acceleration, bool surface, bool density, bool pressure, int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "SPH print");
	float* array = (float*)GpuResources::getDataSSBO(BufferDatails.SPHVariablesName);
	if (array == nullptr) {
		LOG_F(ERROR, "Error while printing SPH DATA");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}
	const int siz = Configuration.MAX_FLUID_PARTICLES;
	for (int i = 0; i < Configuration.MAX_FLUID_PARTICLES && i < limit && i<ParticleData::m_FluidParticlesNum; i++) {
		std::stringstream ss;
		if (velocity) {
			ss << "vel: " << array[3 * i + 0] << "," << array[3 * i + 1] << "," << array[3 * i + 2] << " | ";
		}
		if (acceleration) {
			ss << "acc: " << array[3*siz + 3 * i + 0] << "," << array[3 * siz + 3 * i + 1] << "," << array[3 * siz + 3 * i + 2] << " | ";
		}
		if (surface) {
			ss << "surf: " << array[6 * siz + 3 * i + 0] << "," << array[6 * siz + 3 * i + 1] << "," << array[6 * siz + 3 * i + 2] << " (" << array[9 * siz + i ] << ") | ";
		}
		if (density) {
			ss << "dens: " << array[10 * siz +2*i ] << " | ";
		}
		if (pressure) {
			ss << "press: " << array[10 * siz + 2*i+1] << " | ";
		}
		LOG_F(INFO, "\t %d: %s", i, ss.str().c_str());
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
	FluidParticle* partPositions;
	partPositions = (FluidParticle*)ParticleData::getPositions();

	if (numOfFluidParts > Configuration.MAX_FLUID_PARTICLES) return;
	if (numOfGlassParts > Configuration.MAX_GLASS_PARTICLES) return;

	for (int i = 0; i < numOfFluidParts; i++) {
		partFile << partPositions[i].x << " " << partPositions[i].y << " " << partPositions[i].z << " " << 1 << " ";
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

FluidParticle* ParticleData::getPositions()
{
	return (FluidParticle*)GpuResources::getDataSSBO(BufferDatails.particlePositionsName);
}

float* ParticleData::getGlassPositions()
{
	return (float*)GpuResources::getDataSSBO(BufferDatails.glassPositionsName);
}

float* ParticleData::getGlassVectors()
{
	return (float*)GpuResources::getDataSSBO(BufferDatails.glassVectorName);;
}

unsigned int* ParticleData::getSortingData()
{
	return (unsigned int*)GpuResources::getDataSSBO(BufferDatails.SortVariablesName);
}

int* ParticleData::getNeighboursData()
{
	return (int*)GpuResources::getDataSSBO(BufferDatails.NeighboursListName);
}

ParticleObject* ParticleData::getParticleObjects()
{
	return (ParticleObject*) GpuResources::getDataSSBO(BufferDatails.partObjectsName);
}

SimDetails* ParticleData::getDetails()
{
	return (SimDetails*)GpuResources::getDataSSBO(BufferDatails.detailsName);
}