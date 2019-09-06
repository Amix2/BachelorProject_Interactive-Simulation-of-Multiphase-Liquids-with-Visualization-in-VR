#pragma once
#include <atomic>
#include <Configuration.h>
#include <Logger.h>
#include <shaders/ComputeShader.h>
#include <dataStructures/ParticleData.h>
#include <particleObjects/ParticleObjectCreator.h>
#include <Utils.h>
#include <condition_variable>
#include <mutex>

enum Resource_Request { NO_ORDER, OPEN, COMMIT, COMMIT_AND_OPEN };

class Simulation
{
	inline static ComputeShader shader;
public:

	inline static std::atomic<Resource_Request> m_reqFluidArray		= NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqGlassArray		= NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqGlassVectorsArray = NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqDetils		= NO_ORDER;
	inline static std::atomic<Resource_Request> m_reqObjects	= NO_ORDER;


	// main simulation function
	static void runSimulation();

	static void init();

	// check m_ResourceRequest and perform action
	static void parseResourceRequest();
};

