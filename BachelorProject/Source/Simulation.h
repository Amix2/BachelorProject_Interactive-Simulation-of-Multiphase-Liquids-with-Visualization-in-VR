#pragma once
#include <atomic>
#include <Configuration.h>
#include <Logger.h>
#include <shaders/ComputeShader.h>
#include <dataStructures/GpuResources.h>
#include <dataStructures/ParticleData.h>
#include <Utils.h>

typedef struct NewParticlesArray {
	int particleType;
	int numOfParticles;
	float array[3 * Configuration.MAX_PARTICLES_ADDED_IN_TURN];

} newParticlesArray;

class Simulation
{
	ComputeShader shader;

public:
	/* other thread can put values in this, this class only takes values, sends them to GPU and clears it */
	static NewParticlesArray newParticlesArray;
	/* Mutex protecting flag not shared object */
	inline static std::atomic_bool newPartArrayReady = false;

	Simulation();
	~Simulation() {}
	void runSimulation();
};

