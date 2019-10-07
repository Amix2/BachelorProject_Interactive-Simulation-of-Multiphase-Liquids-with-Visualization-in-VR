#pragma once
#include <glm/gtx/string_cast.hpp>
#include <sstream>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <dataStructures/GpuResources.h>
//#include <particleObjects/ParticleObject.h>
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

struct SimDetails {
	GLuint numOfParticles;
	GLuint numOfGlassParticles;
};

struct Particle {
	GLfloat x, y, z;
	GLint type;
};

struct GlassParticle {
	GLfloat localX, localY, localZ;
	GLfloat vecX, vecY, vecZ;
	GLuint glassNumber;
	float _padding;
};

struct GlassObjectDetails {
	glm::mat4 matrix = glm::mat4(0.0);
};

/* Keeps all details for fluid particles, performs all action with particles */
class ParticleData
{
public:

	//inline static int m_GlassParticlesNum = 0;
	//inline static int m_FluidParticlesNum = 0;
	inline static int m_NumOfParticles = 0;
	inline static int m_NumOfGlassParticles = 0;

	inline static int m_OpenedResources = 0;

	// getters - only this class can change data from gpu
	static Particle* getPositions();
	static GlassParticle* getGlassParticles();
	static GlassObjectDetails* getGlassObjects();
	static unsigned int* getSortingData();
	static int* getNeighboursData();
	static SimDetails* getDetails();


	// resource pointers, array size is currently not used
	inline static Particle* m_resParticlesArray = nullptr;

	inline static GlassParticle* m_resGlassParticleArray = nullptr;

	inline static GlassObjectDetails* m_resGlassObjectsArray = nullptr;
	inline static int m_numOfObjectsInArray = 0;

	inline static SimDetails* m_resDetails = nullptr;

	// mutex and condition variable for threads to wait for resource
	inline static std::condition_variable m_ResourceCondVariable;
	inline static std::mutex m_ResourceMutex;


	/* Init arrays on GPU to store particle data */
	static void initArraysOnGPU();

	static Particle* openParticlePositions();
	static GlassParticle* openGlassParticles();
	static GlassObjectDetails* openGlassObjects();
	static SimDetails* openDetails();


	static void commitParticlePositions(int numOfAddedParticles);
	static void commitGlassParticles(int numOfAddedGlassParticles);
	static void commitGlassObjects(int numOfAddedGlassObjects);
	static void commitDetails();

	static void syncSimDetailsWithGpu();
	static void copyDataForSorting();

	// Prints info about fluid particles
	static void printParticleData(int limit = 10);

	static void printGlassParticlesData(int limit = 10);

	// Prints info about particle objects
	static void printGlassObjectsData(int limit = 10);

	static void printSortingData(int limit = 16);

	static void printNeighboursData(int limit = 10);
	static void printSPHData(bool velocity, bool acceleration, bool surface, bool density, bool pressure, int limit = 10);

	static void logParticlePositions();
	
	
	// for printing particles positions to file
	inline static std::ofstream partFile;

};

