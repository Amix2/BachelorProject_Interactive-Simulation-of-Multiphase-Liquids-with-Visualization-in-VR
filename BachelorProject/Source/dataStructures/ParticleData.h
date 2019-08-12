#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <dataStructures/GpuResources.h>
#include <Configuration.h>
#include <Logger.h>

/* Keeps all details for fluid particles, performs all action with porticles */
class ParticleData
{
	static int m_numOfParticles;
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
public:
	/* Init arrays om GPU to store particle data */
	static void initArraysOnGPU();

	/* Adds n particles with given positions
		1 particle = 3x float in array */
	static void addParticle(const float v3_positions[], int numOfParticlesAdded=1);
};

