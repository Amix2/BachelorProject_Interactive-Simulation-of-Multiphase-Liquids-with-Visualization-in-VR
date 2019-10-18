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
#include <particleObjects/MugParticleObject.h>
#include <particleObjects/ParticleObjectManager.h>


// Allows creating particle objects in worker thread (non blocking for ordering thread)
class ParticleObjectCreator
{
	// worker organization
	inline static std::thread m_workerThread;
	// for waking up worker thread
	inline static std::condition_variable m_condVariable_partObjectDetails;
	inline static std::mutex m_mutex_partObjectDetails;



	static void createFluid(ParticleObjectDetais details, int& numOfParts);

	ParticleObjectCreator() {}
public:

	// data structured for adding order details
	inline static std::vector<ParticleObjectDetais> m_ParticleObjectDetaisVector;	// TODO: make it thread safe

	static void createParticlesFromOrderList();

	// add new order
	static void addObject(ParticleObjectDetais details);

	static bool hasNewOrder();
};

