#include "Simulation.h"


void Simulation::runSimulation()
{
	loguru::set_thread_name("simulation");
	LOG_F(INFO, "simulation running");

	if (Simulation::m_toAddPartArrayReady) {
		// send data to GPU
		LOG_F(INFO, "new particles to send to GPU, type: %d, particles: %d", Simulation::m_toAddParticlesArray.particleType, Simulation::m_toAddParticlesArray.numOfParticles);
		ParticleData::addParticle(Simulation::m_toAddParticlesArray.array, Simulation::m_toAddParticlesArray.particleType, Simulation::m_toAddParticlesArray.numOfParticles);
		Simulation::m_toAddPartArrayReady = false;
	}

	// compute shader (change values)
	long tStart = getTime();
	shader.runShader(1, 1, 1, false);
	long tEnd = getTime();

	
	LOG_F(INFO, "Simulation time: %d", tEnd - tStart);

	checkOpenGLErrors();
}

Simulation::Simulation()
{
	shader = ComputeShader("./Source/shaders/compute.glsl");
}
