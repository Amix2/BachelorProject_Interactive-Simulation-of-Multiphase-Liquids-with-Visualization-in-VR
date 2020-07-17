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
#include <particleObjects/MugParticleObject.h>
#include <Configuration.h>
#include <memory>
#include <selectableObject/SelectableObjectManager.h>

// Holds all particle object in scene, allows selecting and moving objects
class ParticleObjectManager {
	inline static std::atomic_bool m_positionChanged = false;
public:
	inline static std::vector<std::unique_ptr<ParticleObject>> m_partObjectsVector;
	inline static std::atomic_int m_numOfObjects = 0;

	static void moveObject(int objectNumber, float val, glm::vec3 axis);

	static void synchronizeWithGpu();

	static int addObject(std::unique_ptr<MugParticleObject> object);
	static void init();

	static void printObjects(int limit = 5);
};