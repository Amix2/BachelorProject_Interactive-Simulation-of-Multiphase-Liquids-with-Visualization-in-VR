#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Configuration.h>
#include <Logger.h>
#include <dataStructures/ParticleData.h>

// create basic particle structures 
namespace ParticleGeometry {
	enum CalcMode { EQUALS, NOT_EQUALS };
	enum VectorDirection {ZERO, UP, DOWN, INSIDE, OUTSIDE, UP_INSIDE, UP_OUTSIDE, DOWN_INSIDE, DOWN_OUTSIDE};

	//void createFluid(ParticleObjectDetais details, int& numOfParts);
	
	int circle(int& numOfParts, glm::vec3 center, float radius, float inCircleGap, VectorDirection vecDirection);

	int filledCircle(int& numOfParts, glm::vec3 center,
		 float innerRadiusCalc,
		 float innerRadiusRender, CalcMode innerRadiusMode,
		 float outerRadiusCalc,
		 float outerRadiusRender, CalcMode outerRadiusMode,
		 float inCircleGap,
		 float layerGap, VectorDirection mainVecDirection, std::vector<VectorDirection> edgeVecDirections);

	int cylinder(int& numOfParts, glm::vec2 centerLane, float bottomHeight, float topHeight, float radius, float inCircleGap, float layerGap, VectorDirection vecDirection, std::vector<VectorDirection> topVecDirections, std::vector<VectorDirection> bottomVecDirections);

}