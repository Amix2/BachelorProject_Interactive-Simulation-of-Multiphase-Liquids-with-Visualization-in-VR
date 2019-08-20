#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Configuration.h>
#include <Logger.h>

namespace ParticleGeopetry {
	enum CalcMode { EQUALS, NOT_EQUALS };
	
	int circle(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts, glm::vec3 center, float radius, float inCircleGap);

	int filledCircle(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts, glm::vec3 center
		, float innerRadiusCalc, float innerRadiusRender, CalcMode innerRadiusMode
		, float outerRadiusCalc, float outerRadiusRender, CalcMode outerRadiusMode
		, float inCircleGap, float layerGap);

	int cylinder(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts, glm::vec2 centerLane, float bottomHeight, float topHeight, float radius, float inCircleGap, float layerGap);

}