#pragma once
#include <string>
#include <random>

// put on if you want to see particles in Simple Visualizer, it will erase previous content
#define LOG_TO_FILE true

// in case of "inline" errors enable c++17
// https://stackoverflow.com/questions/41308933/how-to-enable-c17-compiling-in-visual-studio

const struct Configuration {
	inline static const int MAX_FLUID_PARTICLES = 10000;
	inline static const int MAX_GLASS_PARTICLES = 10000;
	inline static const int MAX_PARTICLE_OBJECTS = 10;
	inline static const int MAX_FLUID_TYPES = 10;

	inline static const float FLUID_PARTICLE_BUILD_GAP = 0.5f;
	inline static const float GLASS_PARTICLE_BUILD_GAP = 0.5f;

	inline static const float MAX_PARTICLE_SPEED = 0.5f;

	inline static const int NUM_OF_SPH_FLOATS_PER_PARTICLE = 12;

} Configuration;

const struct BufferDatails {
	inline static const std::string particlePositionsName = "ParticlePositions";	// SSBO
	inline static const int particlePositionsBinding = 1;

	inline static const std::string particlesFluidTypeName = "ParticlesFluidTypes";	// SSBO
	inline static const int particlesFluidTypeBinding = 2;

	inline static const std::string glassPositionsName = "GlassPositions";	// SSBO
	inline static const int glassPositionsBinding = 3;

	inline static const std::string glassVectorName = "GlassVector";	// UBO
	inline static const int glassVectorBinding = 4;

	inline static const std::string partObjectsName = "ParticleObjects";	// SSBO
	inline static const int partObjectsBinding = 5;

	inline static const std::string detailsName = "Details";	// SSBO 
	inline static const int detailsBinding = 6;

	inline static const std::string fluidTypesName = "FluidTypes";	// UBO
	inline static const int fluidTypesBinding = 7;

	inline static const std::string SPHVariablesName = "SPHVariables";	// SSBO
	inline static const int SPHVariablesBinding = 8;


} BufferDatails;

class Random {
	inline static std::mt19937 rng;
public:

	static int randInt(int min, int max) {
		std::uniform_int_distribution<uint32_t> int_dist(min, max);
		return int_dist(rng);
	}

	// returns float from [0, 1)
	static float randFloat() {
		std::uniform_int_distribution<uint32_t> int_dist(0, INT_MAX);
		return float(int_dist(rng)) / INT_MAX;
	}
};

