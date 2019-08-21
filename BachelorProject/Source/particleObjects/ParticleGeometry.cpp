#include "ParticleGeometry.h"

inline float gapInPolygon(int vertices, float radius);
inline glm::vec3 partPositionFromRadius(glm::vec3 center, float radius, float angle);

int ParticleGeopetry::circle(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts
	, glm::vec3 center, float radius
	, float inCircleGap)
{
	int numOfPartsInCircle = ceil(2 * M_PI * radius / inCircleGap);
	if (radius == 0) numOfPartsInCircle = 1;
	LOG_F(INFO, "numOfPartsInCircle %d, rad %f", numOfPartsInCircle, radius);
	const float angle = 2 * M_PI / numOfPartsInCircle;
	const float startAngle =  Random::randFloat()* angle;

	float currentAngle = startAngle;
	for (int i = 0; i < numOfPartsInCircle; i++) {

		glm::vec3 thisPos = partPositionFromRadius(center, radius, currentAngle);

		positions[3 * numOfParts + 0] = thisPos.x;
		positions[3 * numOfParts + 1] = thisPos.y;
		positions[3 * numOfParts + 2] = thisPos.z;
		numOfParts++;

		currentAngle += angle;
	}
	return numOfPartsInCircle;
}

int ParticleGeopetry::filledCircle(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts, glm::vec3 center
	, float innerRadiusCalc, float innerRadiusRender, CalcMode innerRadiusMode
	, float outerRadiusCalc, float outerRadiusRender, CalcMode outerRadiusMode
	, float inCircleGap, float layerGap)
{
	const int numOfCircles = ceil((outerRadiusCalc - innerRadiusCalc) / layerGap);
	const float realZoneGap = (outerRadiusCalc - innerRadiusCalc) / numOfCircles;	// gap between circles
	LOG_F(INFO, "numOfCircles %d, innerRadiusCalc %f, outerRadiusCalc %f", numOfCircles, innerRadiusCalc, outerRadiusCalc);

	float currentRadius = innerRadiusCalc;
	for (int i = 0; i <= numOfCircles; i++) {	// we want to do 1 more than numOfCircles cos its number of gaps between circles
		if (	( (innerRadiusMode == EQUALS && innerRadiusRender <= currentRadius)	|| (innerRadiusMode == NOT_EQUALS && innerRadiusRender < currentRadius) )
			&&	( (outerRadiusMode == EQUALS && currentRadius <= outerRadiusRender)	|| (outerRadiusMode == NOT_EQUALS && currentRadius < outerRadiusRender) ) ) {
			LOG_F(INFO, "currentRadius %f, outerRadiusRender %f, mode %d, cond %d", currentRadius, outerRadiusRender, outerRadiusMode, (currentRadius < outerRadiusRender));
			circle(positions, numOfParts, center, currentRadius, inCircleGap);
		}
		currentRadius += realZoneGap;
	}

	return 0;
}

int ParticleGeopetry::cylinder(float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts
	, glm::vec2 centerLane, float bottomHeight, float topHeight, float radius
	, float inCircleGap, float layerGap)
{
	const int numOfCircles = ceil((topHeight - bottomHeight) / layerGap);
	const float realZoneGap = (topHeight - bottomHeight) / numOfCircles;	// gap between circles
	LOG_F(INFO, "numOfLayers %d", numOfCircles);
	float currentHeight = bottomHeight;
	for (int i = 0; i <= numOfCircles; i++) { // we want to do 1 more than numOfCircles cos its number of gaps between circles
		circle(positions, numOfParts, glm::vec3(centerLane.x, currentHeight, centerLane.y), radius, inCircleGap);
		currentHeight += realZoneGap;
	}

	return 0;
}




inline float gapInPolygon(int vertices, float radius)
{
	return sqrt(2 * (1 - cos((2 * M_PI) / vertices)) * radius * radius);
}

inline glm::vec3 partPositionFromRadius(glm::vec3 center, float radius, float angle)
{
	return center + glm::vec3(radius * cos(angle), 0, radius * sin(angle));
}

