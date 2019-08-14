#include "ParticleObjectCreator.h"

void ParticleObjectCreator::runWorkerThread()
{
	loguru::set_thread_name("ParticleObjectCreator worker");
	LOG_F(INFO, "ParticleObjectCreator::runWorkerThread");

	while (true) {
		
		float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN];
		int posInd = 0;
		// check if new data is avaliabie, if not wait, if yes create particles in local array
		std::unique_lock<std::mutex> lck(m_mutex_partObjectDetails);
		while(ParticleObjectCreator::particleObjectDetaisReady == false)	m_condVariable_partObjectDetails.wait(lck);
		lck.unlock();

		LOG_F(INFO, "New object to create: type: %d", ParticleObjectCreator::particleObjectDetais.fluidType);

		positions[0] = particleObjectDetais.lowX;
		positions[1] = particleObjectDetais.lowY;
		positions[2] = particleObjectDetais.lowZ;

		positions[3] = particleObjectDetais.highX;
		positions[4] = particleObjectDetais.highY;
		positions[5] = particleObjectDetais.highZ;

		// check if simulation can take new particles, if no wait, if yes push particles to Simulation::
		std::unique_lock<std::mutex> simLck(m_mutex_partObjectDetails);
		while (Simulation::m_newPartArrayReady == true) Simulation::m_condVariable_newPartArray.wait(simLck);
		simLck.unlock();

		memcpy(&Simulation::m_newParticlesArray.array, positions, 6 * sizeof(float));
		Simulation::m_newParticlesArray.numOfParticles = 2;
		Simulation::m_newParticlesArray.particleType = ParticleObjectCreator::particleObjectDetais.fluidType;
		Simulation::m_newPartArrayReady = true;

		LOG_F(INFO, "New object added to Simulation: type: %d", Simulation::m_newParticlesArray.particleType);

		// loop complete, we are ready to receive next order
		ParticleObjectCreator::particleObjectDetaisReady = false;
	}
}

void ParticleObjectCreator::init()
{
	LOG_F(INFO, "Initialize ParticleObjectCreator");
	ParticleObjectCreator::m_workerThread = std::thread(ParticleObjectCreator::runWorkerThread);
}

void ParticleObjectCreator::addObject(ParticleObjectDetais details)
{
	memcpy(&ParticleObjectCreator::particleObjectDetais, &details, sizeof(ParticleObjectDetais));
	ParticleObjectCreator::particleObjectDetaisReady = true;
	ParticleObjectCreator::m_condVariable_partObjectDetails.notify_all();
	LOG_F(INFO, "New object assign: type: %d", ParticleObjectCreator::particleObjectDetais.fluidType);
}
