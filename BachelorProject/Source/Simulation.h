#pragma once
#include <atomic>
#include <Configuration.h>
#include <Logger.h>
#include <shaders/ComputeShader.h>
#include <dataStructures/GpuResources.h>
#include <dataStructures/ParticleData.h>
#include <Utils.h>
#include <condition_variable>
#include <mutex>

struct NewParticlesArray {
	int particleType;
	int numOfParticles;
	float array[3 * Configuration.MAX_PARTICLES_ADDED_IN_TURN];

};

class Simulation
{
	ComputeShader shader;

public:
	/* other thread can put values in this, this class only takes values, sends them to GPU and clears it */
	inline static NewParticlesArray m_toAddParticlesArray;
	/* Mutex protecting flag not shared object */
	inline static std::atomic_bool m_toAddPartArrayReady = false;

	inline static std::condition_variable m_condVariable_toAddPartArray;
	inline static std::mutex m_mutex_toAddPartArray;

	Simulation();
	~Simulation() {}
	void runSimulation();
};

