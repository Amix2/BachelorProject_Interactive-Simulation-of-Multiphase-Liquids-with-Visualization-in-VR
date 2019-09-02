#include "ParticleObjectCreator.h"

void forceOpenFluid();

void forceOpenGlass();

void forceOpenGlassVectors();

void forceOpenDetails();

void forceOpenObjects();

void weakOpenDetails();

void weakOpenObjects();

void commitFluid();

void commitGlass();

void commitGlassVectors();

void commitDetails();

void commitObjects();

void ParticleObjectCreator::runWorkerThread()
{
	loguru::set_thread_name("ParticleObjectCreator worker");
	LOG_F(INFO, "ParticleObjectCreator::runWorkerThread");

	while (true) {
		LOG_F(INFO, "ParticleObjectCreator::runWorkerThread LOOP");
		// check for new order
		std::unique_lock<std::mutex> lock_ParticleObjectDetails(m_mutex_partObjectDetails);
		while(ParticleObjectCreator::m_ParticleObjectDetaisReady == false)	m_condVariable_partObjectDetails.wait(lock_ParticleObjectDetails);
		lock_ParticleObjectDetails.unlock();

		int numOfParticles = 0;

		// if details is not open request it, we will need it later;
		weakOpenDetails();

		if (m_ParticleObjectDetais.fluidType > 0) {
		////////////////////////////////////////////
		//		FLUID

			forceOpenFluid();

			float* fluidArray = ParticleData::m_resFluidArray;

			ParticleObjectCreator::createFluid(fluidArray, numOfParticles);

			ParticleData::m_numOfAddedFluid = numOfParticles;

			// request commit
			commitFluid();

			// we have to update particle datails in sim (it might not be opened)
			forceOpenDetails();
			ParticleData::m_resDetails->numOfParticles += numOfParticles;
			commitDetails();

		}

		else {
		////////////////////////////////////////////
		//		GLASS
			Sleep(10000);
			// weak open objects array to add particle object
			weakOpenObjects();

			forceOpenGlass();
			forceOpenGlassVectors();

			float* glassPositions = ParticleData::m_resGlassArray;
			float* glassVectors = ParticleData::m_resGlassVectorsArray;

			ParticleObject mug;
			mug.createMug(ParticleObjectCreator::m_ParticleObjectDetais, glassPositions, glassVectors, numOfParticles);

			ParticleData::m_numOfAddedGlass = numOfParticles;

			// request commit
			commitGlass();
			commitGlassVectors();

			// we have to update particle datails in sim (it might not be opened)
			forceOpenDetails();
			ParticleData::m_resDetails->numOfGlassParticles += numOfParticles;
			commitDetails();

			ParticleObjectManager::addObject(mug);
			forceOpenObjects();
			ParticleData::m_resObjectsArray[ParticleData::m_numOfObjectsInArray] = mug;
			ParticleData::m_numOfObjectsInArray++;
			commitObjects();
		}

	
		LOG_F(INFO, "New object added to Simulation: type: %d, particles: %d", m_ParticleObjectDetais.fluidType, numOfParticles);

		// loop complete, we are ready to receive next order
		ParticleObjectCreator::m_ParticleObjectDetaisReady = false;
	}
}

void forceOpenFluid() {
	if (ParticleData::m_resFluidArray == nullptr) {
		// request open 
		std::unique_lock<std::mutex> lock_resource(ParticleData::m_ResourceMutex);	// take resource mutex
		while (ParticleData::m_resFluidArray == nullptr) {
			// while wanted resource in no opened -> order it and wait
			Simulation::m_reqFluidArray = OPEN;
			ParticleData::m_ResourceCondVariable.wait(lock_resource);
		}
		lock_resource.unlock();
	}
}

void forceOpenGlass() {
	if (ParticleData::m_resFluidArray == nullptr) {
		// request open 
		std::unique_lock<std::mutex> lock_resource(ParticleData::m_ResourceMutex);	// take resource mutex
		while (ParticleData::m_resGlassArray == nullptr) {
			// while wanted resource in no opened -> order it and wait
			Simulation::m_reqGlassArray = OPEN;
			ParticleData::m_ResourceCondVariable.wait(lock_resource);
		}
		lock_resource.unlock();
	}
}

void forceOpenGlassVectors()
{
	if (ParticleData::m_resGlassVectorsArray == nullptr) {
		// request open 
		std::unique_lock<std::mutex> lock_resource(ParticleData::m_ResourceMutex);	// take resource mutex
		while (ParticleData::m_resGlassVectorsArray == nullptr) {
			// while wanted resource in no opened -> order it and wait
			Simulation::m_reqGlassVectorsArray = OPEN;
			ParticleData::m_ResourceCondVariable.wait(lock_resource);
		}
		lock_resource.unlock();
	}
}

void forceOpenDetails() {
	if (ParticleData::m_resDetails == nullptr) {
		std::unique_lock<std::mutex> lock_details(ParticleData::m_ResourceMutex);	// take resource mutex
		while (ParticleData::m_resDetails == nullptr) {
			// while wanted resource in no opened -> order it and wait
			Simulation::m_reqDetils = OPEN;
			ParticleData::m_ResourceCondVariable.wait(lock_details);
		}
		lock_details.unlock();
	}
}

void forceOpenObjects()
{
	if (ParticleData::m_resObjectsArray == nullptr) {
		std::unique_lock<std::mutex> lock_details(ParticleData::m_ResourceMutex);	// take resource mutex
		while (ParticleData::m_resObjectsArray == nullptr) {
			// while wanted resource in no opened -> order it and wait
			Simulation::m_reqObjects = OPEN;
			ParticleData::m_ResourceCondVariable.wait(lock_details);
		}
		lock_details.unlock();
	}
}

void weakOpenDetails() {
	if (ParticleData::m_resDetails == nullptr && Simulation::m_reqDetils == NO_ORDER) {
		Simulation::m_reqDetils = OPEN;
	}
}

void weakOpenObjects()
{
	if (ParticleData::m_resObjectsArray == nullptr && Simulation::m_reqObjects == NO_ORDER) {
		Simulation::m_reqObjects = OPEN;
	}
}

void commitFluid()
{
	Simulation::m_reqFluidArray = COMMIT;
}

void commitGlass()
{
	Simulation::m_reqGlassArray = COMMIT;
}

void commitGlassVectors()
{
	Simulation::m_reqGlassVectorsArray = COMMIT;
}

void commitDetails()
{
	Simulation::m_reqDetils = COMMIT;
}

void commitObjects()
{
	Simulation::m_reqObjects = COMMIT;
}

void ParticleObjectCreator::createFluid(float positions[], int &numOfParts) {

	const float gap = Configuration.FLUID_PARTICLE_BUILD_GAP;

	const float startX = ParticleObjectCreator::m_ParticleObjectDetais.lowX;
	const float startY = ParticleObjectCreator::m_ParticleObjectDetais.lowY;
	const float startZ = ParticleObjectCreator::m_ParticleObjectDetais.lowZ;
	const float endX = ParticleObjectCreator::m_ParticleObjectDetais.highX;
	const float endY = ParticleObjectCreator::m_ParticleObjectDetais.highY;
	const float endZ = ParticleObjectCreator::m_ParticleObjectDetais.highZ;

	numOfParts = 0;

	for (float oX = startX; oX <= endX; oX += gap) {
		for (float oY = startY; oY <= endY; oY += gap) {
			for (float oZ = startZ; oZ <= endZ; oZ += gap) {
				positions[3 * numOfParts + 0] = oX;
				positions[3 * numOfParts + 1] = oY;
				positions[3 * numOfParts + 2] = oZ;
				numOfParts++;
			}
		}
	}
}

void ParticleObjectCreator::init()
{
	LOG_F(INFO, "Initialize ParticleObjectCreator");
	ParticleObjectCreator::m_workerThread = std::thread(ParticleObjectCreator::runWorkerThread);
	Threads::addThreadToList(&m_workerThread);
}

bool ParticleObjectCreator::canAddObject()
{
	return m_ParticleObjectDetaisReady == false;
}

void ParticleObjectCreator::addObject(ParticleObjectDetais details)
{
	memcpy(&ParticleObjectCreator::m_ParticleObjectDetais, &details, sizeof(ParticleObjectDetais));
	ParticleObjectCreator::m_ParticleObjectDetaisReady = true;
	ParticleObjectCreator::m_condVariable_partObjectDetails.notify_all();
	LOG_F(INFO, "New object assign: type: %d", ParticleObjectCreator::m_ParticleObjectDetais.fluidType);
}
