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
	inline static std::atomic_bool m_positionChanged = false;
public:
	inline static ParticleObject m_partObjectsArray[Configuration.MAX_PARTICLE_OBJECTS];
	inline static std::atomic_int m_numOfObjects = 0;

	////////////////////////////////////////////////////////////////////////////
	// Methods for user interaction

	glm::vec3* getObjectsPositions();
	glm::vec3* getObjectsDirections();
	// change target position of given object
	static void moveObject(int objectNumber, bool direction);
	////////////////////////////////////////////////////////////////////////////


	static void synchronizeWithGpu();

	static int addObject(const ParticleObject& object);
	static void init();

	static void printObjects(int limit = 5);
};