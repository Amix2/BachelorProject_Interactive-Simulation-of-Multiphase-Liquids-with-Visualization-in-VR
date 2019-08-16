#include "Simulation.h"

void printPositionsToFile();

void Simulation::runSimulation()
{
	loguru::set_thread_name("simulation");
	LOG_F(INFO, "simulation running");

	if (Simulation::m_newPartArrayReady) {
		// send data to GPU
		LOG_F(INFO, "new particles to send to GPU, type: %d, particles: %d", Simulation::m_newParticlesArray.particleType, Simulation::m_newParticlesArray.numOfParticles);
		ParticleData::addParticle(Simulation::m_newParticlesArray.array, Simulation::m_newParticlesArray.particleType, Simulation::m_newParticlesArray.numOfParticles);
		Simulation::m_newPartArrayReady = false;
	}

	// compute shader (change values)
	long tStart = getTime();
	shader.runShader(1, 1, 1, true);
	long tEnd = getTime();

	
	LOG_F(INFO, "Simulation time: %d", tEnd - tStart);

	checkOpenGLErrors();
}

Simulation::Simulation()
{
	shader = ComputeShader("./Source/shaders/compute.glsl");
}

void printPositionsToFile()
{

}
