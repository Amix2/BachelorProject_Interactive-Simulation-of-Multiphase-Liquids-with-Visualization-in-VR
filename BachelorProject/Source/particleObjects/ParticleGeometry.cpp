#include "ParticleGeometry.h"

inline float gapInPolygon(int vertices, float radius);
inline glm::vec3 partPositionFromRadius(glm::vec3 center, float radius, float angle);

int ParticleGeometry::circle(int& numOfParts, glm::vec3 center, float radius, float inCircleGap, VectorDirection vecDirection)
{
	//LOG_F(INFO, "Circle rad %f, part %d", radius, numOfParts);
	int numOfPartsInCircle = (int)std::ceil(2 * M_PI * radius / inCircleGap);
	if (radius == 0) numOfPartsInCircle = 1;
	const float angle = (float) ( 2 * M_PI / numOfPartsInCircle );
	const float startAngle =  Random::randFloat()* angle;

	float currentAngle = startAngle;
	for (int i = 0; i < numOfPartsInCircle; i++) {

		glm::vec3 thisPos = partPositionFromRadius(center, radius, currentAngle);

		//positions[3 * numOfParts + 0] = thisPos.x;
		//positions[3 * numOfParts + 1] = thisPos.y;
		//positions[3 * numOfParts + 2] = thisPos.z;

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
		else if (vecDirection == UP_INSIDE) {
			vec = glm::normalize(glm::vec3(0, 1, 0) + glm::normalize(center - thisPos));
		}
		else if (vecDirection == UP_OUTSIDE) {
			vec = glm::normalize(glm::vec3(0, 1, 0) + glm::normalize(thisPos - center));
		}
		else if (vecDirection == DOWN_INSIDE) {
			vec = glm::normalize(glm::vec3(0, -1, 0) + glm::normalize(center - thisPos));
		}
		else if (vecDirection == DOWN_OUTSIDE) {
			vec = glm::normalize(glm::vec3(0, -1, 0) + glm::normalize(thisPos - center));
		}
		//vectors[3 * numOfParts + 0] = vec.x;
		//vectors[3 * numOfParts + 1] = vec.y;
		//vectors[3 * numOfParts + 2] = vec.z;

		ParticleData::m_resParticlesArray[numOfParts].type = (ParticleData::m_NumOfGlassParticles + numOfParts + 1) * -1;

		ParticleData::m_resGlassParticleArray[numOfParts].localX = thisPos.x;
		ParticleData::m_resGlassParticleArray[numOfParts].localY = thisPos.y;
		ParticleData::m_resGlassParticleArray[numOfParts].localZ = thisPos.z;
		ParticleData::m_resGlassParticleArray[numOfParts].vecX = vec.x;
		ParticleData::m_resGlassParticleArray[numOfParts].vecY = vec.y;
		ParticleData::m_resGlassParticleArray[numOfParts].vecZ = vec.z;
		ParticleData::m_resGlassParticleArray[numOfParts].glassNumber = ParticleData::m_numOfObjectsInArray;

		numOfParts++;

		currentAngle += angle;
	}
	return numOfPartsInCircle;
}

int ParticleGeometry::filledCircle(int& numOfParts, glm::vec3 center
	, float innerRadiusCalc, float innerRadiusRender, CalcMode innerRadiusMode
	, float outerRadiusCalc, float outerRadiusRender, CalcMode outerRadiusMode
	, float inCircleGap,
 float layerGap, VectorDirection mainVecDirection, std::vector<VectorDirection> edgeVecDirections)
{
	//LOG_F(INFO, "filledCircle part %d", numOfParts);
	const int numOfCircles = (int)std::ceil((outerRadiusCalc - innerRadiusCalc) / layerGap);
	const float realZoneGap = (outerRadiusCalc - innerRadiusCalc) / numOfCircles;	// gap between circles

	float currentRadius = innerRadiusCalc;
	for (int i = 0; i <= numOfCircles; i++) {	// we want to do 1 more than numOfCircles cos its number of gaps between circles
		if (	( (innerRadiusMode == EQUALS && innerRadiusRender <= currentRadius)	|| (innerRadiusMode == NOT_EQUALS && innerRadiusRender < currentRadius) )
			&&	( (outerRadiusMode == EQUALS && currentRadius <= outerRadiusRender)	|| (outerRadiusMode == NOT_EQUALS && currentRadius < outerRadiusRender) ) ) {

			VectorDirection vec = mainVecDirection;
			if (numOfCircles - i < 3) {
				vec = edgeVecDirections[numOfCircles - i];
			}
			circle(numOfParts, center, currentRadius, inCircleGap, vec);
		}
		currentRadius += realZoneGap;
	}

	return 0;
}

int ParticleGeometry::cylinder(int& numOfParts, glm::vec2 centerLane, float bottomHeight, float topHeight, float radius, float inCircleGap, float layerGap
	, VectorDirection vecDirection, std::vector<VectorDirection> topVecDirections, std::vector<VectorDirection> bottomVecDirections)
{
	//LOG_F(INFO, "cylinder part %d", numOfParts);
	const int numOfCircles = (int)std::ceil((topHeight - bottomHeight) / layerGap);
	const float realZoneGap = (topHeight - bottomHeight) / numOfCircles;	// gap between circles
	float currentHeight = bottomHeight;
	for (int i = 0; i <= numOfCircles; i++) { // we want to do 1 more than numOfCircles cos its number of gaps between circles
		VectorDirection vecDirForThisCircle = vecDirection;
		if (i < 3) vecDirForThisCircle = bottomVecDirections[i];
		if (i == numOfCircles) vecDirForThisCircle = topVecDirections[i - numOfCircles];
		circle(numOfParts, glm::vec3(centerLane.x, currentHeight, centerLane.y), radius, inCircleGap, vecDirForThisCircle);
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

