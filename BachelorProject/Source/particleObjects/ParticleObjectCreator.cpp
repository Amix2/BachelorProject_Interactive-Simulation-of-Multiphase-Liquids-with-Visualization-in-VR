#include "ParticleObject.h"

void forceOpenFluid();

void forceOpenGlass();

void forceOpenDetails();

void weakOpenDetails();

void commitFluid();

void commitGlass();

void commitDetails();

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

		float* resourcePtr;
		int numOfParticles = 0;

		// if details is not open request it, we will need it later;
		weakOpenDetails();

		if (m_ParticleObjectDetais.fluidType > 0) {
		////////////////////////////////////////////
		//		FLUID

			forceOpenFluid();

			resourcePtr = ParticleData::m_resFluidArray;

			ParticleObjectCreator::createFluid(resourcePtr, numOfParticles);

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

			forceOpenGlass();

			resourcePtr = ParticleData::m_resGlassArray;

			MugObject mug(ParticleObjectCreator::m_ParticleObjectDetais, resourcePtr, numOfParticles);
			ParticleObjectManager::addObject(mug);

			ParticleData::m_numOfAddedGlass = numOfParticles;

			// request commit
			commitGlass();

			// we have to update particle datails in sim (it might not be opened)
			forceOpenDetails();
			ParticleData::m_resDetails->numOfGlassParticles += numOfParticles;
			commitDetails();

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

void weakOpenDetails() {
	if (ParticleData::m_resDetails == nullptr && Simulation::m_reqDetils == NO_ORDER) {
		Simulation::m_reqDetils = OPEN;
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

void commitDetails()
{
	Simulation::m_reqDetils = COMMIT;
}

void ParticleObjectCreator::createFluid(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int &numOfParts) {

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
