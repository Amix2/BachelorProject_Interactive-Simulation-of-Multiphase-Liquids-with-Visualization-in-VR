#include "ParticleGeometry.h"

inline float gapInPolygon(int vertices, float radius);
inline glm::vec3 partPositionFromRadius(glm::vec3 center, float radius, float angle);

int ParticleGeometry::circle(float positions[], float vectors[], int& numOfParts
	, glm::vec3 center, float radius
	, float inCircleGap, VectorDirection vecDirection)
{
	int numOfPartsInCircle = (int)std::ceil(2 * M_PI * radius / inCircleGap);
	if (radius == 0) numOfPartsInCircle = 1;
	const float angle = (float) ( 2 * M_PI / numOfPartsInCircle );
	const float startAngle =  Random::randFloat()* angle;

	float currentAngle = startAngle;
	for (int i = 0; i < numOfPartsInCircle; i++) {

		glm::vec3 thisPos = partPositionFromRadius(center, radius, currentAngle);

		positions[3 * numOfParts + 0] = thisPos.x;
		positions[3 * numOfParts + 1] = thisPos.y;
		positions[3 * numOfParts + 2] = thisPos.z;

		// vector calculation
		glm::vec3 vec(0,0,0);
		if (vecDirection == INSIDE) {
			vec = glm::normalize(center - thisPos);
		}
		else if (vecDirection == OUTSIDE) {
			vec = glm::normalize(thisPos - center);
		}
		else if (vecDirection == UP) {
			vec = glm::vec3(0, 1, 0);
		}
		else if (vecDirection == DOWN) {
			vec = glm::vec3(0, -1, 0);
		}
		else if (vecDirection == ZERO) {
			vec = glm::vec3(0, 0, 0);
		}
		vectors[3 * numOfParts + 0] = vec.x;
		vectors[3 * numOfParts + 1] = vec.y;
		vectors[3 * numOfParts + 2] = vec.z;

		numOfParts++;

		currentAngle += angle;
	}
	return numOfPartsInCircle;
}

int ParticleGeometry::filledCircle(float positions[], float vectors[], int& numOfParts, glm::vec3 center
	, float innerRadiusCalc, float innerRadiusRender, CalcMode innerRadiusMode
	, float outerRadiusCalc, float outerRadiusRender, CalcMode outerRadiusMode
	, float inCircleGap, float layerGap, VectorDirection vecDirection)
{
	const int numOfCircles = (int)std::ceil((outerRadiusCalc - innerRadiusCalc) / layerGap);
	const float realZoneGap = (outerRadiusCalc - innerRadiusCalc) / numOfCircles;	// gap between circles

	float currentRadius = innerRadiusCalc;
	for (int i = 0; i <= numOfCircles; i++) {	// we want to do 1 more than numOfCircles cos its number of gaps between circles
		if (	( (innerRadiusMode == EQUALS && innerRadiusRender <= currentRadius)	|| (innerRadiusMode == NOT_EQUALS && innerRadiusRender < currentRadius) )
			&&	( (outerRadiusMode == EQUALS && currentRadius <= outerRadiusRender)	|| (outerRadiusMode == NOT_EQUALS && currentRadius < outerRadiusRender) ) ) {
			circle(positions, vectors, numOfParts, center, currentRadius, inCircleGap, vecDirection);
		}
		currentRadius += realZoneGap;
	}

	return 0;
}

int ParticleGeometry::cylinder(float positions[], float vectors[], int& numOfParts
	, glm::vec2 centerLane, float bottomHeight, float topHeight, float radius
	, float inCircleGap, float layerGap, VectorDirection vecDirection)
{
	const int numOfCircles = (int)std::ceil((topHeight - bottomHeight) / layerGap);
	const float realZoneGap = (topHeight - bottomHeight) / numOfCircles;	// gap between circles
	float currentHeight = bottomHeight;
	for (int i = 0; i <= numOfCircles; i++) { // we want to do 1 more than numOfCircles cos its number of gaps between circles
		VectorDirection vecDirForThisCircle = vecDirection;
		if (i == 0) vecDirForThisCircle = DOWN;
		if (i == numOfCircles) vecDirForThisCircle = UP;
		circle(positions, vectors, numOfParts, glm::vec3(centerLane.x, currentHeight, centerLane.y), radius, inCircleGap, vecDirForThisCircle);
		currentHeight += realZoneGap;
	}

	return 0;
}




inline float gapInPolygon(int vertices, float radius)
{
	return (float) sqrt(2 * (1 - cos((2 * M_PI) / vertices)) * radius * radius);
}

inline glm::vec3 partPositionFromRadius(glm::vec3 center, float radius, float angle)
{
	return center + glm::vec3(radius * cos(angle), 0, radius * sin(angle));
}

