#include "ParticleObject.h"

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


		const Resource_Type wantedResource = m_ParticleObjectDetais.fluidType < 0 ? GLASS : FLUID;


		// make sure correct resource is opened
		if (ParticleData::m_OpenResource != wantedResource) {

			// its not -> request correct array to be opened
			std::unique_lock<std::mutex> lock_resource(ParticleData::m_ResourceMutex);	// take resource mutex
			
			while (ParticleData::m_OpenResource != wantedResource) {	// while wanted resource is not opened
				if (Simulation::m_ResourceRequest == NO_ORDER) {	// try to ask for it
					if (wantedResource == FLUID)
						Simulation::m_ResourceRequest.store(OPEN_FLUID);
					else
						Simulation::m_ResourceRequest.store(OPEN_GLASS);
				}
				ParticleData::m_ResourceCondVariable.wait(lock_resource);	// wait until something happens	| will wake up when new resource is opened
			}
			lock_resource.unlock();
		}

		// create particles
		int numOfParticles = 0;
		float* positions = ParticleData::m_ResourceArray;

		if (ParticleObjectCreator::m_ParticleObjectDetais.fluidType > 0) {
			ParticleObjectCreator::createFluid(positions, numOfParticles);
		}
		else {
			MugObject mug(ParticleObjectCreator::m_ParticleObjectDetais, positions, numOfParticles);
			ParticleObjectManager::addObject(mug);
		}

		ParticleData::m_AddedParticlesNum = numOfParticles;


		// request commit
		Simulation::m_ResourceRequest.store(COMMIT);

		// check if new data is avaliabie, if not wait, if yes create particles in local array

	
		LOG_F(INFO, "New object added to Simulation: type: %d, particles: %d", m_ParticleObjectDetais.fluidType, numOfParticles);

		// loop complete, we are ready to receive next order
		ParticleObjectCreator::m_ParticleObjectDetaisReady = false;
	}
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
