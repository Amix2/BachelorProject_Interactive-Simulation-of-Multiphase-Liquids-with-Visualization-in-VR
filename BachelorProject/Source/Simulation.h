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

enum Resource_Request { NO_ORDER, OPEN, COMMIT };

class Simulation
{
	ComputeShader shader;

public:

	inline static std::atomic<Resource_Request> m_reqFluidArray;
	inline static std::atomic<Resource_Request> m_reqGlassArray;
	inline static std::atomic<Resource_Request> m_reqDetils;

	Simulation();
	~Simulation() {}

	// main simulation function
	void runSimulation();

	// check m_ResourceRequest and perform action
	void parseResourceRequest();
};

