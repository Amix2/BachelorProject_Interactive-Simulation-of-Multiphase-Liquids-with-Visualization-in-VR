#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <dataStructures/GpuResources.h>
#include <Configuration.h>
#include <Logger.h>
#include <iostream>
#include <fstream>
#include <Utils.h>

typedef struct SimDetails {
	GLuint numOfParticles;
	GLuint numOfGlassParticles;
}SimDetails;


/* Keeps all details for fluid particles, performs all action with porticles */
class ParticleData
{
	inline static int m_numOfAllParticles = 0;
	/*	LIST OF ARRAYS
		for fluid particles = 16x float
		- posiiton (v3)
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

	// private getters - only this class can change data from gpu
	static void* getPositions();
	static void* getGlassPositions();
	static void* getToAddParticlePositions();
	static SimDetails* getDetails();

public:
	/* Init arrays om GPU to store particle data */
	static void initArraysOnGPU();

	//Adds n particles with given positions
	//1 particle = 3x float in array
	static void addParticle(const float v3_positions[], int particleType, int numOfParticlesAdded=1);

	// Prints info about fluid particles
	static void printParticleData(int limit = 10);

	// Prints info about glass particles
	static void printGlassData(int limit = 10);

	// Prints info about to-add particles
	static void printToAddParticleData(int limit = 10);

	static void logParticlePositions();
	
	
	
	inline static std::ofstream partFile;
};

