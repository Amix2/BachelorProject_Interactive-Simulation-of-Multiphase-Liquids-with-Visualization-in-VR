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
#include <particleObjects/ParticleGeometry.h>

struct ParticleObjectDetais {
	int fluidType;	// -1 -> glass | >0 -> index in fluid type array (TODO) | 0 -> reserved
	union {
		struct {
			// for liquid box
			float lowX, lowY, lowZ;
			float highX, highY, highZ;
		};
		struct {
			// for mugs / glass
			float cX, cY, cZ; // center of inner mug
			float innerRadius;
			float thickness;
			float height;
		};
	};
};

// Represents single object like mug etc
class ParticleObject {
	// index of glass particless from this object in global glass particle array (on gpu)


public:
	int m_beginInd, m_endInd;
	glm::vec3 m_position;


	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	std::string print();
};

class MugObject : public ParticleObject {
public:
	MugObject(ParticleObjectDetais details, float positions[], int& numOfParts);
};


// Holds all particle object in scene, allows selecting and moving objects
class ParticleObjectManager {
	inline static std::vector<ParticleObject> m_partObjectsVec;
	inline static int m_numOfObjects = 0;
	inline static int m_selectedObjectIndex = -1;

public:

	static void init();

	static int addObject(const ParticleObject& object);

	static int selectObject(glm::vec3 handPosition);

	static void printObjects(int limit = 5);
};

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

	static void createFluid(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts);

	ParticleObjectCreator() {}
public:

	// data structured for adding order details
	inline static ParticleObjectDetais m_ParticleObjectDetais;
	inline static std::atomic_bool m_ParticleObjectDetaisReady = false; // FALSE -> no new data, can add | TRUE -> new data, worker has to empty it

	// create worker thread
	static void init();

	// check if data structure is ready to receive new order
	static bool canAddObject();

	// add new order
	static void addObject(ParticleObjectDetais details);
};

