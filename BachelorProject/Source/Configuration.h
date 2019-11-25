#pragma once
#include <string>
#include <random>
#include <glm/glm.hpp>

// put on if you want to see particles in Simple Visualizer, it will erase previous content
#define LOG_TO_FILE false

// in case of "inline" errors enable c++17
// https://stackoverflow.com/questions/41308933/how-to-enable-c17-compiling-in-visual-studio

const struct Configuration {
	inline static int MAX_PARTICLES = 2*262144;
	inline static int MAX_GLASS_PARTICLES = 262144/2;
	inline static const int MAX_PARTICLE_OBJECTS = 10;
	inline static const int MAX_PARTICLES_CREATED_IN_TURN = 262144;
	inline static const int MAX_FLUID_TYPES = 10;
	inline static const int MAX_EMITERS = 5;
	inline static const int MAX_SELECTABLE_OBJECTS = 10;

	inline static const int SORT_ARRAY_SIZE = 2*MAX_PARTICLES;
	inline static const float FLUID_PARTICLE_BUILD_GAP = 0.6f;
	inline static const float GLASS_PARTICLE_BUILD_GAP = 0.5f;
	inline static const float EMITER_FLUID_PARTICLE_BUILD_GAP = 0.9f;

	inline static int SCENE_SIZE_X = 9;
	inline static int SCENE_SIZE_Y = 8;
	inline static int SCENE_SIZE_Z = 7;

	inline static const float DELTA_TIME = 0.004f;
	inline static const float GRAVITY_Y = -800;
	inline static const float VELOCITY_FACTOR = 0.7f;
	inline static const float BOUNCE_DISTANCE = 0.50f;
	inline static const float BOUNCE_VELOCITY_MULTIPLIER = 0.5f;

	inline static const float MAX_PARTICLE_SPEED = 1.0f;
	inline static const float MAX_PARTICLE_STEP_DISTANCE = MAX_PARTICLE_SPEED;
	inline static const float MAX_GLASS_PARTICLE_STEP_DISTANCE = 0.3f;

	inline static const int NUM_OF_SPH_FLOATS_PER_PARTICLE = 20;
	inline static const int NUM_OF_SORTING_FLOATS_IN_ARRAY = 2*SORT_ARRAY_SIZE + 9 * MAX_PARTICLES;
	inline static const int SIZE_OF_HELPER_BUFFER = 4*sizeof(float) * MAX_PARTICLES;

	inline static const glm::vec4 BACKGROUND{ 0.1f, 0.1f, 0.4f, 1.0f };
	inline static const int MAX_AMOUNT_OF_TITLES = 10;
	inline static const int TITLE_REFRESH_DELAY = 100;	// in ms

	inline static const float GLASS_ANGLE_PRECISION = 0.0001;
	inline static const float GLASS_DISTANCE_PRECISION = 0.0001;

	inline static const int TARGET_SIM_FPS = 30;
} Configuration;


const struct BufferDetails {
	inline static const std::string particlePositionsName = "ParticlePositions";	// SSBO
	inline static const int particlePositionsBinding = 1;

	inline static const std::string glassParticleName = "GlassParticles";	// SSBO
	inline static const int glassParticleBinding = 2;

	inline static const std::string glassObjectsDetailsName = "GlassObjectsDetails";	// UBO
	inline static const int glassObjectsDetailsBinding = 3;

	inline static const std::string detailsName = "Details";	// SSBO 
	inline static const int detailsBinding = 4;

	inline static const std::string fluidTypesName = "FluidTypes";	// UBO
	inline static const int fluidTypesBinding = 5;

	inline static const std::string NeighboursListName = "NeighboursList";	// SSBO
	inline static const int NeighboursListBinding = 6;

	inline static const std::string SortVariablesName = "SortVariables";	// SSBO
	inline static const int SortVariablesBinding = 7;

	inline static const std::string SPHVariablesName = "SPHVariables";	// SSBO
	inline static const int SPHVariablesBinding = 8;

	inline static const std::string HelperBufferName = "HelperBuffer";	// SSBO
	inline static const int HelperBufferBinding = 9;

	inline static const std::string EmiterBufferName = "EmiterBuffer";	// UBO
	inline static const int EmiterBufferBinding = 10;

} BufferDetails;


const struct ShaderFiles {
	inline static const std::string TEST_ComputeShader = "./Source/shaders/shaderFiles/compute.glsl";
	inline static const std::string BitonicSort = "./Source/shaders/shaderFiles/BitonicSortShader.glsl";
	inline static const std::string CellCountingForSort = "./Source/shaders/shaderFiles/CellCountingForSort.glsl";
	inline static const std::string VariablesArrangementAfterSort = "./Source/shaders/shaderFiles/SortingFinalizer.glsl";
	inline static const std::string SphNeighbourSearch = "./Source/shaders/shaderFiles/SphNeighbourSearch.glsl";	
	inline static const std::string SphNeighbourSearchPreparation = "./Source/shaders/shaderFiles/SphNeighbourSearchPreparation.glsl";
	inline static const std::string SphNeighbourSearchFinalizer = "./Source/shaders/shaderFiles/SphNeighbourSearchFinalizer.glsl";
	inline static const std::string SphDensityPressureFluid = "./Source/shaders/shaderFiles/SphDensityPressureFluid.glsl";
	inline static const std::string SphAccelerationFluid = "./Source/shaders/shaderFiles/SphAccelerationFluid.glsl";
	inline static const std::string SphVelocity = "./Source/shaders/shaderFiles/SphVelocity.glsl";

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

