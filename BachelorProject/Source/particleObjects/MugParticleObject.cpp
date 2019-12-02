#include "MugParticleObject.h"
#include <particleObjects/ParticleObjectVRActionController.h>

glm::vec3 getPerpendicular(const glm::vec3 vec1, const glm::vec3 vec2);

void MugParticleObject::grab()
{
	m_selected = true;
}

void MugParticleObject::release()
{
	m_selected = false;
}

glm::mat4* MugParticleObject::getMatrix()
{
	return &(m_matrix);
}

MugParticleObject::MugParticleObject(ParticleObjectDetais details, int& numOfParts) : ParticleObject() {
	this->create(details, numOfParts);
	m_SelectingRadius = this->m_distanceToFurthestParticle;
	setVRActionController<ParticleObjectVRActionController, MugParticleObject>(this);

}

void MugParticleObject::stepTowardsDestination()
{

	float stepDistanceLeft = Configuration.MAX_GLASS_PARTICLE_STEP_DISTANCE;

	const float maxPositionChange = previousTurnPositionChange + MUG_TURN_VELOCITY_CHANGE * Configuration.DELTA_TIME;
	const float distanceToDestination = glm::distance(Utils::getPosition(m_matrix), Utils::getPosition(m_destinationMatrix));
	const float BOTH_DISTANCE = 10.0f;
	const float LINEAR_DISTANCE = 30.0f;
	float angleCoef;
	if (distanceToDestination < BOTH_DISTANCE)	angleCoef = 1.f;
	else if (distanceToDestination < LINEAR_DISTANCE) angleCoef = (LINEAR_DISTANCE - distanceToDestination) / (LINEAR_DISTANCE - BOTH_DISTANCE) ;
	else angleCoef = 0.f;
	angleCoef = 1.f;
	float stepDistanceChange = min(maxPositionChange, Configuration.MAX_GLASS_PARTICLE_STEP_DISTANCE*0.5f);

	float linearPositionChange = 0;

	// move max distance = min( Configuration.MAX_GLASS_PARTICLE_STEP_DISTANCE * 0.5, maxPositionChange )
	const float positionMove1 = stepPositionChange(stepDistanceChange);

	stepDistanceLeft -= positionMove1;
	linearPositionChange = positionMove1;
	previousTurnPositionChange = positionMove1;

	// angle move, max distance = stepDistanceLeft * angleCoef 
	const float angleMove = stepAngleChange(stepDistanceLeft * angleCoef);

	stepDistanceLeft -= angleMove;

	// move max distance = min( stepDistanceLeft, maxPositionChange - linearPositionChange)
	const float positionMove2 = stepPositionChange(min(stepDistanceLeft, maxPositionChange - linearPositionChange));
	previousTurnPositionChange += positionMove2;
}


float MugParticleObject::stepPositionChange(const float maxDistance)
{
	const glm::vec3 currPos = Utils::getPosition(m_matrix);
	const glm::vec3 destPos = Utils::getPosition(m_destinationMatrix);
	const glm::vec3 difVec = destPos - currPos;
	const float distance = glm::length(difVec);
	const float stepDistanceChange = min(distance, maxDistance);

	float linearPositionChange = 0;

	if (distance > Configuration.GLASS_DISTANCE_PRECISION) {
		const glm::vec3 stepVec = glm::normalize(difVec) * stepDistanceChange;
		m_matrix[3][0] += stepVec.x;
		m_matrix[3][1] += stepVec.y;
		m_matrix[3][2] += stepVec.z;
		linearPositionChange = stepDistanceChange;
	}
	return linearPositionChange;
}

float MugParticleObject::stepAngleChange(const float maxDistance)
{
	glm::vec3 currUp = glm::normalize(Utils::getUp(m_matrix));
	glm::vec3 destUp = glm::normalize(Utils::getUp(m_destinationMatrix));
	const float totalAngle = glm::angle(currUp, destUp);
	const float angleX = glm::orientedAngle(currUp, destUp, glm::vec3(1, 0, 0));
	const float angleY = glm::orientedAngle(currUp, destUp, glm::vec3(0, 1, 0));
	const float angleZ = glm::orientedAngle(currUp, destUp, glm::vec3(0, 0, 1));
	const float maxAngleInStep = atan2f(maxDistance, m_distanceToFurthestParticle);
	float angleChange = min(totalAngle, maxAngleInStep);
	if (angleChange > Configuration.GLASS_ANGLE_PRECISION) {
		if (angleChange > M_PI) angleChange -= 2*M_PI;

		//if (destUp.y < 0) perpVec *= -1;
		currUp = Utils::getUp(m_matrix);
		destUp = Utils::getUp(m_destinationMatrix);
		glm::vec3 perpVec = getPerpendicular(currUp, destUp);
		//LOG_F(WARNING, "perp befor\t%s", glm::to_string(perpVec).c_str());
		//m_matrix = glm::rotate(m_matrix, angleChange, glm::normalize(glm::cross(currUp, destUp)));
		//m_matrix = m_matrix * glm::rotate(glm::mat4{ 1 }, angleChange, glm::normalize(glm::cross(currUp, destUp)));
		//Utils::setUp(&m_matrix, destUp);
		//m_matrix = m_matrix * glm::rotate(glm::mat4{ 1 }, angleChange, glm::vec3(0, 0, 1));

		const glm::vec3 position = Utils::getPosition(m_matrix);
		Utils::setPosition(&m_matrix, { 0,0,0 });
		m_matrix = Utils::getRotationMatrix(perpVec, -angleChange) * m_matrix;
		Utils::setPosition(&m_matrix, position);

		//currUp = Utils::getUp(m_matrix);
		//destUp = Utils::getUp(m_destinationMatrix);
		//perpVec = getPerpendicular(currUp, destUp);
		//LOG_F(WARNING, "perp after\t%s", glm::to_string(perpVec).c_str());
		return tanf(angleChange) * m_distanceToFurthestParticle;
	}
	return 0.0f;
}


glm::vec3 getPerpendicular(const glm::vec3 vec1, const glm::vec3 vec2) {
	float crossX = vec1.y * vec2.z - vec1.z * vec2.y;
	float crossY = vec1.z * vec2.x - vec1.x * vec2.z;
	float crossZ = vec1.x * vec2.y - vec1.y * vec2.x;

	float length = (float)sqrt(crossX * crossX +
		crossY * crossY +
		crossZ * crossZ);

	if (length > 0)
		return glm::vec3(crossX / length, crossY / length, crossZ / length);
	else
		return glm::vec3(1,0,0);
}

void MugParticleObject::create(ParticleObjectDetais details, int& numOfParts)
{
	const float inCircleGap = Configuration.GLASS_PARTICLE_BUILD_GAP; // gap on oX, in a circle
	const float layerGap = (float)inCircleGap * (float)sqrt(3) * 0.5f;	// gap on oY
	const float innerRadius = details.innerRadius;
	const glm::vec3 localCenter = glm::vec3(0, 0, 0);
	const glm::vec3 objectCenter = glm::vec3(details.cX, details.cY, details.cZ);
	const float thickness = details.thickness;
	const float height = details.height;
	const float top = localCenter.y + height / 2;
	const glm::vec3 topVec = localCenter + glm::vec3(0, height / 2, 0);
	const float bottom = localCenter.y - height / 2;
	const glm::vec3 bottomVec = localCenter - glm::vec3(0, height / 2, 0);
	const glm::vec3 layerGapVecY = glm::vec3(0, layerGap, 0);

	float outputThickness = -1;

	// set up attributes
	//m_matrix = glm::rotate(glm::translate(glm::mat4(1.0f), objectCenter), 0.1f, glm::vec3(1,0,0));
	m_matrix = glm::translate(glm::mat4(1.0f), objectCenter);
	// glm::translate(m_matrix, objectCenter);//
	m_destinationMatrix = m_matrix;
	//m_destinationMatrix =glm::translate(m_matrix, glm::vec3(0,100,0));

	m_innerRadius = innerRadius;
	m_height = height;



	// x->cylinders || v->filled Circles
	// are drown allways from the right side towards middle (2nd condition: from top to bottom looking at blob of the same type)
	if (thickness <= layerGap) {
		/*
				x x         x x
				x x         x x
				x x  v v v  x x
				x x  v v v  x x
		*/
		outputThickness = layerGap;

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top		// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE
			, { ParticleGeometry::UP, ParticleGeometry::OUTSIDE, ParticleGeometry::OUTSIDE }
		, { ParticleGeometry::DOWN, ParticleGeometry::OUTSIDE, ParticleGeometry::OUTSIDE });

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE
			, { ParticleGeometry::UP, ParticleGeometry::INSIDE, ParticleGeometry::INSIDE }
		, { ParticleGeometry::DOWN, ParticleGeometry::INSIDE, ParticleGeometry::INSIDE });


		ParticleGeometry::filledCircle(numOfParts
			, bottomVec
			// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP
			, { ParticleGeometry::UP_INSIDE, ParticleGeometry::UP_INSIDE, ParticleGeometry::UP });

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY
			// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::DOWN
			, { ParticleGeometry::DOWN, ParticleGeometry::DOWN, ParticleGeometry::DOWN });

	}
	else if (thickness <= 2 * layerGap) {
		/*
				  x			    x
				x x x         x x x
				x x x         x x x
				x x x  v v v  x x x
				x x x  v v v  x x x
				x x  v v v v v  x x
		*/

		outputThickness = 2 * layerGap;

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 2 * layerGap, top - layerGap / 2		// heights
			, innerRadius + 2 * layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE
			, { ParticleGeometry::UP_OUTSIDE, ParticleGeometry::OUTSIDE, ParticleGeometry::OUTSIDE }
		, { ParticleGeometry::DOWN_OUTSIDE, ParticleGeometry::OUTSIDE, ParticleGeometry::OUTSIDE });

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, top - 2 * layerGap, top 	// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE
			, { ParticleGeometry::UP, ParticleGeometry::INSIDE, ParticleGeometry::INSIDE }
		, { ParticleGeometry::ZERO, ParticleGeometry::ZERO, ParticleGeometry::ZERO });

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top - layerGap / 2	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE
			, { ParticleGeometry::UP_INSIDE, ParticleGeometry::INSIDE, ParticleGeometry::INSIDE }
		, { ParticleGeometry::UP_INSIDE, ParticleGeometry::UP_INSIDE, ParticleGeometry::UP_INSIDE });


		ParticleGeometry::filledCircle(numOfParts
			, bottomVec
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP
			, { ParticleGeometry::UP_INSIDE, ParticleGeometry::UP_INSIDE, ParticleGeometry::UP_INSIDE });

		//ParticleGeometry::filledCircle(numOfParts
		//	, bottomVec - layerGapVecY
		//	, 0, 0, ParticleGeometry::EQUALS		// inner
		//	, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
		//	, inCircleGap, layerGap, ParticleGeometry::UP
		//	, { ParticleGeometry::UP, ParticleGeometry::UP, ParticleGeometry::UP });

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius + 2 * layerGap, innerRadius + 1.5f * layerGap, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::DOWN
			, { ParticleGeometry::DOWN, ParticleGeometry::DOWN, ParticleGeometry::DOWN });
	}
	else if (thickness <= 3 * layerGap) {
		/*
				  x	x			  x x
				x x x x         x x x x
				x x x x         x x x x
				x x x x  v v v  x x x x
				x x x x  v v v  x x x x
				x x  v v v v v v v  x x
				x x  v v v v v v v  x x

		*/

		//ParticleGeometry::cylinder(numOfParts
		//	, glm::vec2(localCenter.x, localCenter.z)
		//	, bottom - 3 * layerGap, top - layerGap		// heights
		//	, innerRadius + 3 * layerGap	// radius
		//	, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		//ParticleGeometry::cylinder(numOfParts
		//	, glm::vec2(localCenter.x, localCenter.z)
		//	, bottom - 3 * layerGap, top 	// heights
		//	, innerRadius + 2 * layerGap	// radius
		//	, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		//ParticleGeometry::cylinder(numOfParts
		//	, glm::vec2(localCenter.x, localCenter.z)
		//	, bottom - layerGap, top 	// heights
		//	, innerRadius + layerGap	// radius
		//	, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here

		//ParticleGeometry::cylinder(numOfParts
		//	, glm::vec2(localCenter.x, localCenter.z)
		//	, bottom - layerGap, top - layerGap	// heights
		//	, innerRadius				// radius
		//	, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here


		//ParticleGeometry::filledCircle(numOfParts
		//	, bottomVec
		//	, 0, 0, ParticleGeometry::EQUALS		// inner
		//	, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
		//	, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		//ParticleGeometry::filledCircle(numOfParts
		//	, bottomVec - layerGapVecY
		//	, 0, 0, ParticleGeometry::EQUALS		// inner
		//	, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
		//	, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		//ParticleGeometry::filledCircle(numOfParts
		//	, bottomVec - layerGapVecY - layerGapVecY
		//	, 0, 0, ParticleGeometry::EQUALS		// inner
		//	, innerRadius + 2 * layerGap, innerRadius + 1.5 * layerGap, ParticleGeometry::NOT_EQUALS	// outer
		//	, inCircleGap, layerGap, ParticleGeometry::DOWN);	// always should be here

		//ParticleGeometry::filledCircle(numOfParts
		//	, bottomVec - layerGapVecY - layerGapVecY - layerGapVecY
		//	, 0, 0, ParticleGeometry::EQUALS		// inner
		//	, innerRadius + 2 * layerGap, innerRadius + 1.5 * layerGap, ParticleGeometry::NOT_EQUALS	// outer
		//	, inCircleGap, layerGap, ParticleGeometry::DOWN);	// always should be here

	}
	else if (thickness <= 4 * layerGap) {
		/*
				  x  v v v v  x             x  v v v v  x
				x x  v v v v  x x         x x  v v v v  x x
				x x			  x x         x x		    x x
				x x			  x x         x x		    x x
				x x			  x x  v v v  x x		    x x
				x x			  x x  v v v  x x		    x x
				x x									    x x
				x x									    x x
				x x									    x x
				x x									    x x
				x x	 v v v v v v v v v v v v v v v v v  x x
				x x	 v v v v v v v v v v v v v v v v v  x x

		*/

		throw "TODO, thickness is too big, lower it or implement this mug";
	}


	m_thickness = outputThickness;
	for (int i = numOfParts - 1; i >= 0; i--) {
		const glm::vec3 partPos = glm::vec3(ParticleData::m_resGlassParticleArray[i].localX, ParticleData::m_resGlassParticleArray[i].localY, ParticleData::m_resGlassParticleArray[i].localZ);
		const float distance = glm::distance(localCenter, partPos);
		m_distanceToFurthestParticle = max(m_distanceToFurthestParticle, distance);
	}
}
