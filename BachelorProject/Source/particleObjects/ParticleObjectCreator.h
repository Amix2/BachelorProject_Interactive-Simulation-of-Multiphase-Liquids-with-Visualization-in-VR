#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <Logger.h>
#include <Simulation.h>
#include <ThreadManager.h>
#include <Configuration.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <particleObjects/ParticleObject.h>
#include <particleObjects/ParticleObjectManager.h>


// Allows creating particle objects in worker thread (non blocking for ordering thread)
class ParticleObjectCreator
{
	// worker organization
	inline static std::thread m_workerThread;
	// for waking up worker thread
	inline static std::condition_variable m_condVariable_partObjectDetails;
	inline static std::mutex m_mutex_partObjectDetails;

	// main function for worker thread
	static void runWorkerThread();

	static void createFluid(ParticleObjectDetais details, float positions[], int fluidTypes[], int& numOfParts);

	ParticleObjectCreator() {}
public:

	// data structured for adding order details
	//inline static ParticleObjectDetais m_ParticleObjectDetais;
	inline static std::vector<ParticleObjectDetais> m_ParticleObjectDetaisVector;
	//inline static std::atomic_bool m_ParticleObjectDetaisReady = false; // FALSE -> no new data, can add | TRUE -> new data, worker has to empty it

	// create worker thread
	static void init();

	// add new order
	static void addObject(ParticleObjectDetais details);
};
