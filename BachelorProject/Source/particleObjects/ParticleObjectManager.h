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
#include <Configuration.h>

// Holds all particle object in scene, allows selecting and moving objects
class ParticleObjectManager {
	// worker organization
	//inline static std::thread m_workerThread;
	inline static std::atomic_bool m_positionChanged = false;
public:
	inline static ParticleObject m_partObjectsArray[Configuration.MAX_PARTICLE_OBJECTS];
	inline static int m_numOfObjects = 0;

	// change target position of given object
	void moveObject(int objectNumber, glm::vec3 targetPosition, glm::vec3 targetDirection);

	glm::vec3* getObjectsPositions();
	glm::vec3* getObjectsDirections();

	static void synchronizeWithGpu();

	static int addObject(const ParticleObject& object);
	static void init();



	static void printObjects(int limit = 5);
};