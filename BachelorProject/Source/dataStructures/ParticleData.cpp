#include "ParticleData.h"

//ParticleObject* ParticleData::m_resObjectsArray = (ParticleObject*)nullptr;

void ParticleData::initArraysOnGPU()
{
	// create SSBO for particle positions
	GpuResources::createSSBO(BufferDetails.particlePositionsName, 4 * (long)Configuration.MAX_PARTICLES * sizeof(float), NULL, BufferDetails.particlePositionsBinding);

	// create UBO for glass particles (local pos, surface vectors)
	GpuResources::createSSBO(BufferDetails.glassParticleName, Configuration.MAX_GLASS_PARTICLES * sizeof(GlassParticle), NULL, BufferDetails.glassParticleBinding);
	// create SSBO for glass positions

	// create UBO for GlassObjectsDetails (matrix)
	GlassObjectDetails objectsArray[Configuration.MAX_PARTICLE_OBJECTS];
	GpuResources::createUBO(BufferDetails.glassObjectsDetailsName, Configuration.MAX_PARTICLE_OBJECTS * sizeof(GlassObjectDetails), objectsArray, BufferDetails.glassObjectsDetailsBinding);

	// create UBO for Fluid types data (fuilled with data, pre-defined types)
	GpuResources::createUBO(BufferDetails.fluidTypesName, Configuration.MAX_FLUID_TYPES * sizeof(FluidType), FluidType::m_fluidTypes, BufferDetails.fluidTypesBinding);

	// create SSBO for simulation details
	SimDetails simDetails{ 0,0 };
	GpuResources::createSSBO(BufferDetails.detailsName, sizeof(simDetails), &simDetails, BufferDetails.detailsBinding);

	// SSBO for SPH
	GpuResources::createSSBO(BufferDetails.SPHVariablesName, (GLsizeiptr)Configuration.MAX_PARTICLES * Configuration.NUM_OF_SPH_FLOATS_PER_PARTICLE * sizeof(float), NULL, BufferDetails.SPHVariablesBinding);

	// SSBO for sorting
	GpuResources::createSSBO(BufferDetails.SortVariablesName, Configuration.NUM_OF_SORTING_FLOATS_IN_ARRAY * sizeof(float), NULL, BufferDetails.SortVariablesBinding);

	// SSBO for neighbour list
	GpuResources::createSSBO(BufferDetails.NeighboursListName, (GLsizeiptr)27 * Configuration.MAX_PARTICLES * sizeof(GLuint), NULL, BufferDetails.NeighboursListBinding);

	// SSBO for neighbour look up table
	GpuResources::createSSBO(BufferDetails.HelperBufferName, (GLsizeiptr)Configuration.SCENE_SIZE_X * Configuration.SCENE_SIZE_Y * Configuration.SCENE_SIZE_Z * sizeof(GLint), NULL, BufferDetails.HelperBufferBinding);

	checkOpenGLErrors();

}

Particle* ParticleData::openParticlePositions__MAP__()
{
	m_ParticleBufferOpen = true;
	LOG_F(INFO, "OPEN to add array for PARTICLES");
	m_resParticlesArray__MAP__ = (Particle*)GpuResources::openPartSSBO__MAP__(BufferDetails.particlePositionsName
		, (GLintptr)m_NumOfParticles * sizeof(Particle)	// offset
		, ((GLsizeiptr)Configuration.MAX_PARTICLES - m_NumOfParticles) * sizeof(Particle)	// length
	);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resParticlesArray__MAP__;
}

GlassParticle* ParticleData::openGlassParticles__MAP__()
{
	LOG_F(INFO, "OPEN to add array for GLASS PARTICLES");
	m_resGlassParticleArray__MAP__ = (GlassParticle*)GpuResources::openPartSSBO__MAP__(BufferDetails.glassParticleName
		, (GLintptr)m_NumOfGlassParticles * sizeof(GlassParticle)	// offset
		, ((GLsizeiptr)Configuration.MAX_GLASS_PARTICLES - m_NumOfGlassParticles) * sizeof(GlassParticle)	// length
	);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resGlassParticleArray__MAP__;
}


GlassObjectDetails* ParticleData::openGlassObjects__MAP__()
{
	//LOG_F(INFO, "OPEN to add array for GLASS OBJECTS");
	m_resGlassObjectsArray__MAP__ = (GlassObjectDetails*)GpuResources::openUBO__MAP__(BufferDetails.glassObjectsDetailsName);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resGlassObjectsArray__MAP__;
}

SimDetails* ParticleData::openDetails__MAP__()
{
	//LOG_F(INFO, "OPEN details struct");
	m_resDetails__MAP__ = (SimDetails*)GpuResources::openSSBO__MAP__(BufferDetails.detailsName);

	m_OpenedResources++;
	ParticleData::m_ResourceCondVariable.notify_all();
	return m_resDetails__MAP__;
}

void ParticleData::commitParticlePositions__MAP__(int numOfAddedParticles)
{
	LOG_F(INFO, "COMMIT PARTICLES");

	GpuResources::commitSSBO__MAP__(BufferDetails.particlePositionsName);
	m_NumOfParticles += numOfAddedParticles;
	m_resParticlesArray__MAP__ = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();

	m_ParticleBufferOpen = false;
}

void ParticleData::commitGlassParticles__MAP__(int numOfAddedGlassParticles)
{
	LOG_F(INFO, "COMMIT GLASS PARTICLES");

	GpuResources::commitSSBO__MAP__(BufferDetails.glassParticleName);
	m_NumOfGlassParticles += numOfAddedGlassParticles;
	m_resGlassParticleArray__MAP__ = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitGlassObjects__MAP__(int numOfAddedGlassObjects)
{
	//LOG_F(INFO, "COMMIT GLASS OBJECTS");

	GpuResources::commitUBO__MAP__(BufferDetails.glassObjectsDetailsName);
	m_numOfObjectsInArray += numOfAddedGlassObjects;
	m_resGlassObjectsArray__MAP__ = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}

void ParticleData::commitDetails__MAP__()
{
	//LOG_F(INFO, "COMMIT details");

	GpuResources::commitSSBO__MAP__(BufferDetails.detailsName);

	m_resDetails__MAP__ = nullptr;
	m_OpenedResources--;
	ParticleData::m_ResourceCondVariable.notify_all();
}


void ParticleData::sendParticlePositions(int numOfAddedParticles)
{
	LOG_F(INFO, "sendParticlePositions %d", numOfAddedParticles);
	GpuResources::updateBuffer(BufferDetails.particlePositionsName, m_NumOfParticles * sizeof(Particle), numOfAddedParticles * sizeof(Particle), m_resParticlesArray.get());
	m_NumOfParticles += numOfAddedParticles;
}

void ParticleData::sendGlassParticles(int numOfAddedGlassParticles)
{
	LOG_F(INFO, "sendGlassParticles %d", numOfAddedGlassParticles);
	GpuResources::updateBuffer(BufferDetails.glassParticleName, m_NumOfGlassParticles * sizeof(GlassParticle), numOfAddedGlassParticles * sizeof(GlassParticle), m_resGlassParticleArray.get());
	m_NumOfGlassParticles += numOfAddedGlassParticles;
}

void ParticleData::sendGlassObjects(int numOfAddedGlassObjects)
{
	LOG_F(INFO, "sendGlassObjects %d", numOfAddedGlassObjects);
	GpuResources::updateBuffer(BufferDetails.glassObjectsDetailsName, 0, m_numOfObjectsInArray * sizeof(GlassObjectDetails), m_resGlassObjectsArray.get());
	m_numOfObjectsInArray += numOfAddedGlassObjects;
}

void ParticleData::sendDetails()
{
	LOG_F(INFO, "sendDetails");
	GpuResources::updateBuffer(BufferDetails.detailsName, 0, sizeof(SimDetails), m_resDetails.get());
}

void ParticleData::syncSimDetailsWithGpu()
{
	//m_resDetails = std::move(ParticleData::getDetails());
	//m_NumOfGlassParticles = m_resDetails->numOfGlassParticles;
	//m_NumOfParticles = m_resDetails->numOfParticles;
	ParticleData::openDetails__MAP__();
	const SimDetails gpuDetails = *ParticleData::m_resDetails__MAP__;
	if (gpuDetails.numOfParticles != ParticleData::m_NumOfParticles || gpuDetails.numOfGlassParticles != ParticleData::m_NumOfGlassParticles) {
		//LOG_F(INFO, "GPU changed num of particles, gpu: (%d, %d) -> cpu: (%d, %d)", gpuDetails.numOfParticles, gpuDetails.numOfGlassParticles, ParticleData::m_NumOfParticles, ParticleData::m_NumOfGlassParticles);
		ParticleData::m_NumOfParticles = gpuDetails.numOfParticles;
		ParticleData::m_NumOfGlassParticles = gpuDetails.numOfGlassParticles;
	}
	ParticleData::commitDetails__MAP__();
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
	const int cpyPosSize = Configuration.MAX_PARTICLES * sizeof(Particle);
	const int cpyPosSizeUsed = ParticleData::m_NumOfParticles * sizeof(Particle);
	const int cpyVelSize = Configuration.MAX_PARTICLES * 3 * sizeof(float);;
	const int cpyVelSizeUsed = ParticleData::m_NumOfParticles * 3*sizeof(float);
	const std::string sortTargetName = BufferDetails.SortVariablesName;
	const std::string positionsSourceName = BufferDetails.particlePositionsName;
	const std::string velositySourceName = BufferDetails.SPHVariablesName;
	//const std::string typesSourceName = BufferDetails.particlesFluidTypeName;

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
	std::unique_ptr<Particle[]> data = ParticleData::getPositions();
	
	LOG_F(INFO, "Simulation particles print\tNum of particles in simulation: %d", m_NumOfParticles);
	for (int i = 0; i < Configuration.MAX_PARTICLES && i < m_NumOfParticles && i < limit; i++) {
		LOG_F(INFO, "%d:\t( %.4f  %.4f  %.4f ) [%d]", i, data[i].x, data[i].y, data[i].z, data[i].type);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	/*SimDetails* details = getDetails();
	Particle* partPositions = ParticleData::getPositions();

	if (details == nullptr || partPositions == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation fluid particles");
		return;
	}

	if (details == nullptr || partPositions == nullptr) {
		LOG_F(ERROR, "Error while printing Simulation particles");
		return;
	}

	if (limit < 0) {
		limit = INT_MAX;
	}

	LOG_F(INFO, "\tNum of particles in simulation: %d, on CPU side: %d", details->numOfParticles, m_NumOfParticles);
	for (int i = 0; i < Configuration.MAX_PARTICLES && ((i < details->numOfParticles || i < m_NumOfParticles) && i < limit); i++) {
		LOG_F(INFO, "%d:\t( %.4f  %.4f  %.4f ) [%d]", i, partPositions[i].x, partPositions[i].y, partPositions[i].z, partPositions[i].type);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");*/
}

void ParticleData::printGlassParticlesData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation glass particles print");
	std::unique_ptr<GlassParticle[]> glassPart = ParticleData::getGlassParticles();

	if (limit <= 0) {
		limit = INT_MAX;
	}

	LOG_F(INFO, "\tNum of glass particles in simulation: %d", m_NumOfGlassParticles);
	for (int i = 0; i < Configuration.MAX_PARTICLES && i < m_NumOfParticles && i < limit; i++) {
		LOG_F(INFO, "%d:\tloc: ( %.4f  %.4f  %.4f ) vec: [ %.4f  %.4f  %.4f ][%u]", i, glassPart[i].localX, glassPart[i].localY, glassPart[i].localZ, glassPart[i].vecX, glassPart[i].vecY, glassPart[i].vecZ, glassPart[i].glassNumber);
	}
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}


void ParticleData::printGlassObjectsData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "Simulation Glass Objects print");
	std::unique_ptr<GlassObjectDetails[]> glassObjects = ParticleData::getGlassObjects();
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
	auto array = ParticleData::getSortingData();
	if (array == nullptr) {
		LOG_F(ERROR, "Error while printing SORTING DATA");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}

	unsigned int prevValue = INT_MAX;
	for (int i = 0; i < Configuration.SORT_ARRAY_SIZE && i < limit; i++) {
		LOG_F(INFO, "\t %d: %u\t[%u]", i, array[i], array[Configuration.SORT_ARRAY_SIZE + i]);
	}
	bool sorted = true;
	for (int i = 0; i < Configuration.SORT_ARRAY_SIZE; i++) {
		if (array[i] > prevValue) sorted = false;
		prevValue = array[i];
	}
	LOG_F(INFO, "SORTED: %s", sorted ? "true" : "false");
	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::printNeighboursData(int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "NEIGHBOURS print");
	auto array = ParticleData::getNeighboursData();
	if (array == nullptr) {
		LOG_F(ERROR, "Error while printing NEIGHBOURS DATA");
		return;
	}
	if (limit <= 0) {
		limit = INT_MAX;
	}

	for (int i = 0; i < Configuration.MAX_PARTICLES && i < limit && i < ParticleData::m_NumOfParticles; i++) {
		std::stringstream ss;
		bool fluid = false;
		for (int x = 0; x < 27; x++) {
			ss << array[27 * i + x] << ", ";
			if (array[27 * i + x] != 0) fluid = true;
		}
		if (fluid) {
			LOG_F(INFO, "\t %d: %s", i, ss.str().c_str());
		}
		else {
			limit++;
		}
	}

	LOG_F(INFO, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

void ParticleData::printSPHData(bool velocity, bool acceleration, bool surface, bool density, bool pressure, int limit)
{
	LOG_F(INFO, "==============================");
	LOG_F(INFO, "SPH print");
	float* array = (float*)GpuResources::getDataSSBO__MAP__(BufferDetails.SPHVariablesName);
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
	//if (ParticleData::m_resParticlesArray__MAP__ != nullptr) {
	//	return;
	//}
	//SimDetails* details;
	//if (ParticleData::m_resDetails__MAP__ == nullptr) {
	//	details = ParticleData::getDetails();
	//}
	//else {
	//	details = ParticleData::m_resDetails__MAP__;
	//}
	//const int numOfFluidParts = details->numOfParticles;
	//const int numOfGlassParts = details->numOfGlassParticles;
	//std::unique_ptr<Particle[]> partPositions = ParticleData::getPositions();

	//if (numOfFluidParts > Configuration.MAX_PARTICLES) return;
	//if (numOfGlassParts > Configuration.MAX_GLASS_PARTICLES) return;

	//for (int i = 0; i < numOfFluidParts; i++) {
	//	partFile << partPositions[i].x << " " << partPositions[i].y << " " << partPositions[i].z << " " << 1 << " ";
	//}


	//partFile << "|";
}

void ParticleData::checkDensity()
{
	float* array = (float*)GpuResources::getDataSSBO__MAP__(BufferDetails.SPHVariablesName);
	std::unique_ptr<Particle[]> partPositions = ParticleData::getPositions();
	if (array == nullptr) {
		LOG_F(ERROR, "Error while printing SPH DATA");
		return;
	}
	const float limit = 90.0f;
	const int siz = Configuration.MAX_PARTICLES;
	for (int i = 0; i < Configuration.MAX_PARTICLES && i < ParticleData::m_NumOfParticles; i++) {
		if (partPositions[i].type > 0 && array[10 * siz + 2 * i] < limit && partPositions[i].y > 2) LOG_F(WARNING, "density below : %f (%f, %f, %f)", array[10 * siz + 2 * i], partPositions[i].x, partPositions[i].y, partPositions[i].z);
	}
}

////////////////////////////////////////////////////////
//	private getters

std::unique_ptr<Particle[]> ParticleData::getPositions()
{
	//return (Particle*)GpuResources::getDataSSBO__MAP__(BufferDetails.particlePositionsName);
	std::unique_ptr<Particle[]> ptr = std::make_unique<Particle[]>(m_NumOfParticles);
	GpuResources::getBufferData(BufferDetails.particlePositionsName, 0, m_NumOfParticles * sizeof(Particle), ptr.get());
	return std::move(ptr);
}

std::unique_ptr<GlassParticle[]> ParticleData::getGlassParticles()
{
	std::unique_ptr<GlassParticle[]> ptr = std::make_unique<GlassParticle[]>(m_NumOfGlassParticles);
	GpuResources::getBufferData(BufferDetails.glassParticleName, 0, m_NumOfGlassParticles * sizeof(GlassParticle), ptr.get());
	return std::move(ptr);
}

std::unique_ptr<GlassObjectDetails[]> ParticleData::getGlassObjects()
{
	std::unique_ptr<GlassObjectDetails[]> ptr = std::make_unique<GlassObjectDetails[]>(m_NumOfParticles);
	GpuResources::getBufferData(BufferDetails.glassObjectsDetailsName, 0, m_numOfObjectsInArray * sizeof(GlassObjectDetails), ptr.get());
	return std::move(ptr);
}

std::unique_ptr<GLuint[]> ParticleData::getSortingData()
{
	std::unique_ptr<GLuint[]> ptr = std::make_unique<GLuint[]>(Configuration.NUM_OF_SORTING_FLOATS_IN_ARRAY);
	GpuResources::getBufferData(BufferDetails.SortVariablesName, 0, Configuration.NUM_OF_SORTING_FLOATS_IN_ARRAY * sizeof(GLuint), ptr.get());
	return std::move(ptr);
}

std::unique_ptr<int[]> ParticleData::getNeighboursData()
{
	std::unique_ptr<int[]> ptr = std::make_unique<int[]>((GLsizeiptr)27 * m_NumOfParticles);
	GpuResources::getBufferData(BufferDetails.NeighboursListName, 0, (GLsizeiptr)27 * m_NumOfParticles * sizeof(GLuint), ptr.get());
	return std::move(ptr);
}

std::unique_ptr<SimDetails> ParticleData::getDetails()
{
	std::unique_ptr<SimDetails> ptr = std::make_unique<SimDetails>();
	GpuResources::getBufferData(BufferDetails.detailsName, 0, sizeof(SimDetails), ptr.get());
	return std::move(ptr);
}

//unsigned int* ParticleData::getSortingData()
//{
//	return (unsigned int*)GpuResources::getDataSSBO__MAP__(BufferDetails.SortVariablesName);
//}
//
//int* ParticleData::getNeighboursData()
//{
//	return (int*)GpuResources::getDataSSBO__MAP__(BufferDetails.NeighboursListName);
//}
//
//GlassObjectDetails* ParticleData::getGlassObjects()
//{
//	return (GlassObjectDetails*) GpuResources::getDataUBO__MAP__(BufferDetails.glassObjectsDetailsName);
//}
//
//SimDetails* ParticleData::getDetails()
//{
//	return (SimDetails*)GpuResources::getDataSSBO__MAP__(BufferDetails.detailsName);
//}
