#pragma once
#include <string>
#include <random>

// put on if you want to see particles in Simple Visualizer, it will erase previous content
#define LOG_TO_FILE false

// in case of "inline" errors enable c++17
// https://stackoverflow.com/questions/41308933/how-to-enable-c17-compiling-in-visual-studio

const struct Configuration {
	inline static const int MAX_FLUID_PARTICLES = 2048;
	inline static const int MAX_GLASS_PARTICLES = 2048;
	inline static const int MAX_PARTICLE_OBJECTS = 10;
	inline static const int MAX_FLUID_TYPES = 10;
	inline static const int SORT_ARRAY_SIZE = 2*MAX_FLUID_PARTICLES;
	inline static const float FLUID_PARTICLE_BUILD_GAP = 1.0f;
	inline static const float GLASS_PARTICLE_BUILD_GAP = 0.5f;

	inline static const float MAX_PARTICLE_SPEED = 0.5f;

	inline static const int NUM_OF_SPH_FLOATS_PER_PARTICLE = 12;
	inline static const int NUM_OF_SORTING_FLOATS_IN_ARRAY = 2*SORT_ARRAY_SIZE + 7 * MAX_FLUID_PARTICLES;

} Configuration;


const struct BufferDatails {
	inline static const std::string particlePositionsName = "ParticlePositions";	// SSBO
	inline static const int particlePositionsBinding = 1;

	//inline static const std::string particlesFluidTypeName = "ParticlesFluidTypes";	// SSBO
	//inline static const int particlesFluidTypeBinding = 2;

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

	inline static const std::string SortVariablesName = "SortVariables";	// SSBO
	inline static const int SortVariablesBinding = 9;


} BufferDatails;


const struct ShaderFiles {
	inline static const std::string TEST_ComputeShader = "./Source/shaders/compute.glsl";
	inline static const std::string BitonicSort = "./Source/shaders/BitonicSortShader.glsl";
	inline static const std::string CellCountingForSort = "./Source/shaders/CellCountingForSort.glsl";
	inline static const std::string VariablesArrangementAfterSort = "./Source/shaders/SortingFinalizer.glsl";
	

} ShaderFiles;



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

