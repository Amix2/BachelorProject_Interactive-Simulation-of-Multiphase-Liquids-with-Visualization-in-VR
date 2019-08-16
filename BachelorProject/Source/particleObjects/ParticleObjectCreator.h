#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <Logger.h>
#include <Simulation.h>
#include <ThreadManager.h>

struct ParticleObjectDetais {
	int fluidType;	// -1 -> glass | >=0 -> index in fluid type array (TODO)
	union {
		struct {
			// for liquid box
			float lowX, lowY, lowZ;
			float highX, highY, highZ;
		};
		struct {
			// for mugs / glass
			int other;
		};
	};
};

class ParticleObjectCreator
{
	// worker organization
	inline static std::thread m_workerThread;
	// for waking up worker thread
	inline static std::condition_variable m_condVariable_partObjectDetails;
	inline static std::mutex m_mutex_partObjectDetails;

	// main function for worker thread
	static void runWorkerThread();

	ParticleObjectCreator() {}
public:

	// data structured for adding order details
	inline static ParticleObjectDetais particleObjectDetais;
	inline static std::atomic_bool particleObjectDetaisReady = false; // FALSE -> no new data, can add | TRUE -> new data, worker has to empty it

	// create worker thread
	static void init();

	// check if data structure is ready to receive new order
	static bool canAddObject();

	// add new order
	static void addObject(ParticleObjectDetais details);
};

