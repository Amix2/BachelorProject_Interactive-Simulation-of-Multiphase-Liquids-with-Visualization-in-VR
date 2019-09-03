#pragma once
#include <string>
#include <random>

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

} Configuration;

const struct BufferDatails {
	inline static const std::string particlePositionsName = "ParticlePositions";	// SSBO
	inline static const int particlePositionsBinding = 10;

	inline static const std::string glassPositionsName = "GlassPositions";	// SSBO
	inline static const int glassPositionsBinding = 11;

	inline static const std::string partObjectsName = "ParticleObjects";	// SSBO
	inline static const int partObjectsBinding = 12;

	inline static const std::string detailsName = "Details";	// SSBO 
	inline static const int detailsBinding = 13;

	inline static const std::string glassVectorName = "GlassVector";	// UBO
	inline static const int glassVectorBinding = 14;

	inline static const std::string fluidTypesName = "FluidTypes";	// UBO
	inline static const int fluidTypesBinding = 15;

	inline static const std::string particlesFluidTypeName = "ParticlesFluidTypes";	// SSBO
	inline static const int particlesFluidTypeBinding = 16;
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

