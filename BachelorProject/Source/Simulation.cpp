#include "Simulation.h"

Simulation::Simulation()
{
	shader = ComputeShader("./Source/shaders/compute.glsl");

	ParticleData::initArraysOnGPU();
}

void Simulation::runSimulation()
{
	loguru::set_thread_name("simulation");
	LOG_F(INFO, "simulation start");

	// create particles
	float pos[901];
	for (int i = 0; i < 901; i++) {
		pos[i] = i * 100;
	}

	// push particles to GPU storage (SSBO)
	ParticleData::addParticle(pos, 3);

	// compute shader (change values)
	long tStart = getTime();
	shader.runShader(1, 1, 1, true);
	long tEnd = getTime();

	LOG_F(INFO, "Simulation time: %d", tEnd - tStart);

	// by defauls its illegal to have access to raw data from GPU, only class ParticleData can access it
	float* p = (float*)GpuResources::getDataSSBO(BufferDatails.particlePositionsName);
	for (int i = 0; i < 10; i++) {
		std::cout << "after compute shader " << p[i] << std::endl;
	}
	checkOpenGLErrors();
}
