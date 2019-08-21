#include "ParticleObject.h"

void ParticleObjectCreator::runWorkerThread()
{
	loguru::set_thread_name("ParticleObjectCreator worker");
	LOG_F(INFO, "ParticleObjectCreator::runWorkerThread");

	while (true) {
		
		float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN];
		int numOfParticles = 0;
		// check if new data is avaliabie, if not wait, if yes create particles in local array
		std::unique_lock<std::mutex> lck(m_mutex_partObjectDetails);
		while(ParticleObjectCreator::particleObjectDetaisReady == false)	m_condVariable_partObjectDetails.wait(lck);
		lck.unlock();

		LOG_F(INFO, "New object to create: type: %d", ParticleObjectCreator::particleObjectDetais.fluidType);

		if (ParticleObjectCreator::particleObjectDetais.fluidType > 0) {
			ParticleObjectCreator::createFluid(positions, numOfParticles);
		}
		else {
			MugObject(ParticleObjectCreator::particleObjectDetais, positions, numOfParticles);
		}

		LOG_F(INFO, "New object created: particles: %d", numOfParticles);

		// check if simulation can take new particles, if no wait, if yes push particles to Simulation::
		std::unique_lock<std::mutex> simLck(m_mutex_partObjectDetails);
		while (Simulation::m_toAddPartArrayReady == true) Simulation::m_condVariable_toAddPartArray.wait(simLck);
		simLck.unlock();

		memcpy(&Simulation::m_toAddParticlesArray.array, positions, (size_t)numOfParticles *3 * sizeof(float));
		Simulation::m_toAddParticlesArray.numOfParticles = numOfParticles;
		Simulation::m_toAddParticlesArray.particleType = ParticleObjectCreator::particleObjectDetais.fluidType;
		Simulation::m_toAddPartArrayReady = true;

		LOG_F(INFO, "New object added to Simulation: type: %d", Simulation::m_toAddParticlesArray.particleType);

		// loop complete, we are ready to receive next order
		ParticleObjectCreator::particleObjectDetaisReady = false;
	}
}

void ParticleObjectCreator::createFluid(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int &numOfParts) {

	const float gap = Configuration.FLUID_PARTICLE_BUILD_GAP;

	const float startX = ParticleObjectCreator::particleObjectDetais.lowX;
	const float startY = ParticleObjectCreator::particleObjectDetais.lowY;
	const float startZ = ParticleObjectCreator::particleObjectDetais.lowZ;
	const float endX = ParticleObjectCreator::particleObjectDetais.highX;
	const float endY = ParticleObjectCreator::particleObjectDetais.highY;
	const float endZ = ParticleObjectCreator::particleObjectDetais.highZ;

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
	return particleObjectDetaisReady == false;
}

void ParticleObjectCreator::addObject(ParticleObjectDetais details)
{
	memcpy(&ParticleObjectCreator::particleObjectDetais, &details, sizeof(ParticleObjectDetais));
	ParticleObjectCreator::particleObjectDetaisReady = true;
	ParticleObjectCreator::m_condVariable_partObjectDetails.notify_all();
	LOG_F(INFO, "New object assign: type: %d", ParticleObjectCreator::particleObjectDetais.fluidType);
}
