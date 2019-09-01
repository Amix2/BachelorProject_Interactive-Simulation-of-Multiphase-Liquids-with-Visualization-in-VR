#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Configuration.h>
#include <Logger.h>

namespace ParticleGeometry {
	enum CalcMode { EQUALS, NOT_EQUALS };
	enum VectorDirection {ZERO, UP, DOWN, INSIDE, OUTSIDE};
	
	int circle(float positions[], float vectors[], int& numOfParts, glm::vec3 center, float radius, float inCircleGap, VectorDirection vecDirection);

	int filledCircle(float positions[], float vectors[], int& numOfParts, glm::vec3 center
		, float innerRadiusCalc, float innerRadiusRender, CalcMode innerRadiusMode
		, float outerRadiusCalc, float outerRadiusRender, CalcMode outerRadiusMode
		, float inCircleGap, float layerGap, VectorDirection vecDirection);

	int cylinder(float positions[], float vectors[], int& numOfParts, glm::vec2 centerLane, float bottomHeight, float topHeight, float radius, float inCircleGap, float layerGap, VectorDirection vecDirection);

}