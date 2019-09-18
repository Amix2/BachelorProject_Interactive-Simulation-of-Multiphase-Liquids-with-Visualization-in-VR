#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <dataStructures/GpuResources.h>
#include <particleObjects/ParticleObject.h>
#include <dataStructures/FluidType.h>
#include <algorithm>
#include <Configuration.h>
#include <Logger.h>
#include <iostream>
#include <fstream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <Utils.h>

/*	LIST OF ARRAYS
	for fluid particles = 16x float
	- position (v3)
	- velocity (v3)
	- acceleration (v3)
	- surfaceNormalVector (v3)
	- surfaceDistance (1f)
	- density (1f)
	- pressure (1f)
	- cellIndex (1i)

	for glass particles	 = 6x float
	- position (v3)
	- normalVector (v3)
*/

typedef struct SimDetails {
	GLuint numOfParticles;
	GLuint numOfGlassParticles;
}SimDetails;

class ParticleObject;	// forward-declare

/* Keeps all details for fluid particles, performs all action with particles */
class ParticleData
{
public:

	inline static int m_GlassParticlesNum = 0;
	inline static int m_FluidParticlesNum = 0;

	// getters - only this class can change data from gpu
	static float* getPositions();
	static float* getGlassPositions();
	static float* getGlassVectors();
	static unsigned int* getSortingData();
	static ParticleObject* getParticleObjects();
	static SimDetails* getDetails();

	// resource pointers, array size is currently not used
	inline static float* m_resFluidArray = nullptr;
	inline static int* m_resFluidTypesArray = nullptr;
	inline static int m_resFluidArraySize;
	inline static std::atomic_int m_numOfAddedFluid;

	inline static float* m_resGlassArray = nullptr;
	inline static float* m_resGlassVectorsArray = nullptr;
	inline static int m_resGlassArraySize;
	inline static std::atomic_int m_numOfAddedGlass;

	inline static ParticleObject* m_resObjectsArray = nullptr;
	inline static int m_resObjectsArraySize;
	inline static int m_numOfObjectsInArray = 0;

	inline static SimDetails* m_resDetails = nullptr;

	// mutex and condition vaciable for threads to wait for resource
	inline static std::condition_variable m_ResourceCondVariable;
	inline static std::mutex m_ResourceMutex;


	/* Init arrays on GPU to store particle data */
	static void initArraysOnGPU();

	static void openFluidArray();
	static void openGlassArray();
	static void openGlassVectors();
	static void openDetails();
	static void openObjects();

	static void commitFluidArray();
	static void commitGlassArray();
	static void commitGlassVectors();
	static void commitDetails();
	static void commitObjects();

	// Prints info about fluid particles
	static void printParticleData(int limit = 10);

	// Prints info about glass particles
	static void printGlassData(int limit = 10);

	static void printGlassVectorsData(int limit = 10);

	// Prints info about particle objects
	static void printParticleObjectsData(int limit = 10);

	static void printSortingData();

	static void logParticlePositions();
	
	
	// for printing particles positions to file
	inline static std::ofstream partFile;

};

