#include "ParticleData.h"

//ParticleObject* ParticleData::m_resObjectsArray = (ParticleObject*)nullptr;

void ParticleData::initArraysOnGPU()
{
	// create SSBO for particle positions
	GpuResources::createSSBO(BufferDatails.particlePositionsName, 4 * Configuration.MAX_PARTICLES * sizeof(float), NULL, BufferDatails.particlePositionsBinding);

	// create UBO for glass particles (local pos, surface vectors)
	GpuResources::createUBO(BufferDatails.glassParticleName, Configuration.MAX_GLASS_PARTICLES * sizeof(GlassParticle), NULL, BufferDatails.glassParticleBinding);
	// create SSBO for glass positions

	// create UBO for GlassObjectsDetails (matrix)
	GlassObjectDetails objectsArray[Configuration.MAX_PARTICLE_OBJECTS];
	GpuResources::createUBO(BufferDatails.glassObjectsDetailsName, Configuration.MAX_PARTICLE_OBJECTS * sizeof(GlassObjectDetails), objectsArray, BufferDatails.glassObjectsDetailsBinding);

	// create UBO for Fluid types data (fuilled with data, pre-defined types)
	GpuResources::createUBO(BufferDatails.fluidTypesName, Configuration.MAX_FLUID_TYPES * sizeof(FluidType), FluidType::m_fluidTypes, BufferDatails.fluidTypesBinding);

	// create SSBO for simulation details
	SimDetails simDetails{ 0,0 };
	GpuResources::createSSBO(BufferDatails.detailsName, sizeof(simDetails), &simDetails, BufferDatails.detailsBinding);

	// SSBO for SPH
	GpuResources::createSSBO(BufferDatails.SPHVariablesName, (GLsizeiptr)Configuration.MAX_PARTICLES * Configuration.NUM_OF_SPH_FLOATS_PER_PARTICLE * sizeof(float), NULL, BufferDatails.SPHVariablesBinding);

	// SSBO for sorting
	GpuResources::createSSBO(BufferDatails.SortVariablesName, Configuration.NUM_OF_SORTING_FLOATS_IN_ARRAY * sizeof(float), NULL, BufferDatails.SortVariablesBinding);

	// SSBO for neighbour list
	GpuResources::createSSBO(BufferDatails.NeighboursListName, (GLsizeiptr)27 * Configuration.MAX_PARTICLES * sizeof(GLuint), NULL, BufferDatails.NeighboursListBinding);

	checkOpenGLErrors();
}

Particle* ParticleData::openParticlePositions()
{
	LOG_F(INFO, "OPEN to add array for PARTICLES");
	m_resParticlesArray = (Particle*)GpuResources::openPartSSBO(BufferDatails.particlePositionsName
		, (GLintptr)m_NumOfParticles * sizeof(Particle)	// offset
		, ((GLsizeiptr)Configuration.MAX_PARTICLES - m_NumOfParticles) * sizeof(Particle)	// length
	);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resParticlesArray;
}

GlassParticle* ParticleData::openGlassParticles()
{
	LOG_F(INFO, "OPEN to add array for GLASS PARTICLES");
	m_resGlassParticleArray = (GlassParticle*)GpuResources::openPartUBO(BufferDatails.glassParticleName
		, (GLintptr)m_NumOfGlassParticles * sizeof(GlassParticle)	// offset
		, ((GLsizeiptr)Configuration.MAX_GLASS_PARTICLES - m_NumOfGlassParticles) * sizeof(GlassParticle)	// length
	);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resGlassParticleArray;
}


GlassObjectDetails* ParticleData::openGlassObjects()
{
	//LOG_F(INFO, "OPEN to add array for GLASS OBJECTS");
	m_resGlassObjectsArray = (GlassObjectDetails*)GpuResources::openUBO(BufferDatails.glassObjectsDetailsName);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resGlassObjectsArray;
}

SimDetails* ParticleData::openDetails()
{
	//LOG_F(INFO, "OPEN details struct");
	m_resDetails = (SimDetails*)GpuResources::openSSBO(BufferDatails.detailsName);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resDetails;
}

void ParticleData::commitParticlePositions(int numOfAddedParticles)
{
	LOG_F(INFO, "COMMIT PARTICLES");

	GpuResources::commitSSBO(BufferDatails.particlePositionsName);
	m_NumOfParticles += numOfAddedParticles;
	m_resParticlesArray = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitGlassParticles(int numOfAddedGlassParticles)
{
	LOG_F(INFO, "COMMIT GLASS PARTICLES");

	GpuResources::commitUBO(BufferDatails.glassParticleName);
	m_NumOfGlassParticles += numOfAddedGlassParticles;
	m_resGlassParticleArray = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitGlassObjects(int numOfAddedGlassObjects)
{
	//LOG_F(INFO, "COMMIT GLASS OBJECTS");

	GpuResources::commitUBO(BufferDatails.glassObjectsDetailsName);
	m_numOfObjectsInArray += numOfAddedGlassObjects;
	m_resGlassObjectsArray = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitDetails()
{
	//LOG_F(INFO, "COMMIT details");

	GpuResources::commitSSBO(BufferDatails.detailsName);

	m_resDetails = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}


void ParticleData::syncSimDetailsWithGpu()
{
	ParticleData::openDetails();
	const SimDetails gpuDetails = *ParticleData::m_resDetails;
	if (gpuDetails.numOfParticles != ParticleData::m_NumOfParticles || gpuDetails.numOfGlassParticles != ParticleData::m_NumOfGlassParticles) {
		LOG_F(INFO, "GPU changed num of particles, gpu: (%d, %d) -> cpu: (%d, %d)", gpuDetails.numOfParticles, gpuDetails.numOfGlassParticles, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles);
		ParticleData::m_NumOfParticles = gpuDetails.numOfParticles;
		ParticleData::m_NumOfGlassParticles = gpuDetails.numOfGlassParticles;
	}
	ParticleData::commitDetails();
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
	const int cpyPosSize = 4 * Configuration.MAX_PARTICLES * sizeof(float);
	const int cpyPosSizeUsed = ParticleData::m_NumOfParticles * sizeof(Particle);
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
	Particle* partPositions = ParticleData::getPositions();

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

	LOG_F(INFO, "\tNum of particles in simulation: %d, on CPU side: %d", details->numOfParticles, m_NumOfParticles);
	for (int i = 0; i < Configuration.MAX_PARTICLES && (i < details->numOfParticles || i < m_NumOfParticles) && i < limit; i++) {
		LOG_F(INFO, "%d:\t( %.4f  %.4f  %.4f ) [%d]", i, partPositions[i].x, partPositions[i].y, partPositions[i].z, partPositions[i].type);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::printGlassParticlesData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation glass particles print");
	SimDetails* details = getDetails();
	GlassParticle* glassPart = ParticleData::getGlassParticles();

	if (glassPart == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation glass particles (glass)");
		return;
	}

	if (details == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation glass particles (details)");
		return;
	}

	if (limit <= 0) {
		limit = INT_MAX;
	}

	LOG_F(INFO, "\tNum of glass particles in simulation: %d, on CPU side: %d", details->numOfGlassParticles, m_NumOfGlassParticles);
	for (int i = 0; i < Configuration.MAX_PARTICLES && (i < details->numOfParticles || i < m_NumOfParticles) && i < limit; i++) {
		LOG_F(INFO, "%d:\tloc: ( %.4f  %.4f  %.4f ) vec: [ %.4f  %.4f  %.4f ][%u]", i, glassPart[i].localX, glassPart[i].localY, glassPart[i].localZ, glassPart[i].vecX, glassPart[i].vecY, glassPart[i].vecZ, glassPart[i].glassNumber);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}


void ParticleData::printGlassObjectsData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation Glass Objects print");
	GlassObjectDetails* glassObjects = ParticleData::getGlassObjects();
	if (glassObjects == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation Glass Objects");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}

	for (int i = 0; i < Configuration.MAX_PARTICLE_OBJECTS && i < limit; i++) {
		LOG_F(INFO, "%d: %s", i, glm::to_string(glassObjects[i].matrix).c_str());
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

	for (int i = 0; i < Configuration.MAX_PARTICLES && i < limit && i < ParticleData::m_NumOfParticles; i++) {
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
	const int siz = Configuration.MAX_PARTICLES;
	for (int i = 0; i < Configuration.MAX_PARTICLES && i < limit && i<ParticleData::m_NumOfParticles; i++) {
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
	if (ParticleData::m_resParticlesArray != nullptr) {
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
	Particle* partPositions;
	partPositions = (Particle*)ParticleData::getPositions();

	if (numOfFluidParts > Configuration.MAX_PARTICLES) return;
	if (numOfGlassParts > Configuration.MAX_GLASS_PARTICLES) return;

	for (int i = 0; i < numOfFluidParts; i++) {
		partFile << partPositions[i].x << " " << partPositions[i].y << " " << partPositions[i].z << " " << 1 << " ";
	}


	partFile << "|";
}

////////////////////////////////////////////////////////
//	private getters

Particle* ParticleData::getPositions()
{
	return (Particle*)GpuResources::getDataSSBO(BufferDatails.particlePositionsName);
}

GlassParticle* ParticleData::getGlassParticles()
{
	return (GlassParticle*)GpuResources::getDataUBO(BufferDatails.glassParticleName);
}

unsigned int* ParticleData::getSortingData()
{
	return (unsigned int*)GpuResources::getDataSSBO(BufferDatails.SortVariablesName);
}

int* ParticleData::getNeighboursData()
{
	return (int*)GpuResources::getDataSSBO(BufferDatails.NeighboursListName);
}

GlassObjectDetails* ParticleData::getGlassObjects()
{
	return (GlassObjectDetails*) GpuResources::getDataUBO(BufferDatails.glassObjectsDetailsName);
}

SimDetails* ParticleData::getDetails()
{
	return (SimDetails*)GpuResources::getDataSSBO(BufferDatails.detailsName);
}
