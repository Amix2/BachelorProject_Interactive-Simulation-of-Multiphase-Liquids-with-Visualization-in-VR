#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <Logger.h>
#include <Simulation.h>

struct ParticleObjectDetais {
	int fluidType;	// -1 -> glass | >=0 -> index in fluid type array (TODO)
	union {
		struct {
			// cors of fluid box
			float lowX, lowY, lowZ;
			float highX, highY, highZ;
		 };
		struct {
			int other;
		};
	};
};

class ParticleObjectCreator
{
	inline static std::thread m_workerThread;
	inline static std::condition_variable m_condVariable_partObjectDetails;
	inline static std::mutex m_mutex_partObjectDetails;

	static void runWorkerThread();

	ParticleObjectCreator() {}
public:

	inline static ParticleObjectDetais particleObjectDetais;
	inline static std::atomic_bool particleObjectDetaisReady = false; // FALSE -> no new data, can add | TRUE -> new data, worker has to empty it

	static void init();
	static void addObject(ParticleObjectDetais details);
};

