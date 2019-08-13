#pragma once
#include <string>
// in case of "inline" errors enable c++17
// https://stackoverflow.com/questions/41308933/how-to-enable-c17-compiling-in-visual-studio

const struct Configuration {
	inline static const int MAX_FLUID_PARTICLES = 2048;
	inline static const int MAX_GLASS_PARTICLES = 1024;
	inline static const int MAX_PARTICLES_ADDED_IN_TURN = 1024;
} Configuration;

const struct BufferDatails {
	inline static const std::string particlePositionsName = "ParticlePositions";
	inline static const int particlePositionsBinding = 10;
} BufferDatails;

