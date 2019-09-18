#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

struct ParticleVectors {
	glm::vec4 glassVectors;
	glm::vec4 glassMovements;

	int indBegin;
	int indEnd;

	float _padding[2];
};
