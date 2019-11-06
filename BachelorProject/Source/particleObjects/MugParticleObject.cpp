#include "MugParticleObject.h"

MugParticleObject::MugParticleObject(ParticleObjectDetais details, int& numOfParts) {
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
//glm::translate(m_matrix, objectCenter);
	m_destinationMatrix = m_matrix;

	m_center = objectCenter;
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

		outputThickness = 2*layerGap;

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
			, innerRadius + 2*layerGap, innerRadius + 1.5f * layerGap, ParticleGeometry::NOT_EQUALS	// outer
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

inline glm::vec3 getPerpendicular(const glm::vec3 vec1, const glm::vec3 vec2) {
	float crossX = vec1.y * vec2.z - vec1.z * vec2.y;
	float crossY = vec1.z * vec2.x - vec1.x * vec2.z;
	float crossZ = vec1.x * vec2.y - vec1.y * vec2.x;

	float length = (float)sqrt(crossX * crossX +
		crossY * crossY +
		crossZ * crossZ);

	if (length > 0)
		return glm::vec3(crossX / length, crossY / length, crossZ / length);
	else
		return glm::vec3();
}

void MugParticleObject::stepTowardsDestination()
{
	float stepDistanceLeft = Configuration.MAX_GLASS_PARTICLE_STEP_DISTANCE;
	const glm::vec3 currPos = glm::vec3(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
	const glm::vec3 destPos = glm::vec3(m_destinationMatrix[3][0], m_destinationMatrix[3][1], m_destinationMatrix[3][2]);
	const glm::vec3 difVec = destPos - currPos;
	const float distance = glm::length(difVec);
	float stepDistanceChange = min(distance, Configuration.MAX_GLASS_PARTICLE_STEP_DISTANCE/2);
	if (distance > Configuration.GLASS_DISTANCE_PRECISION) {
		const glm::vec3 stepVec = glm::normalize(difVec) * stepDistanceChange;
		//m_matrix = m_matrix * glm::translate(m_matrix, stepVec);
		m_matrix[3][0] += stepVec.x;
		m_matrix[3][1] += stepVec.y;
		m_matrix[3][2] += stepVec.z;
		stepDistanceLeft -= stepDistanceChange;
	}

	const glm::vec3 currUp = glm::vec3(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
	const glm::vec3 destUp = glm::vec3(m_destinationMatrix[1][0], m_destinationMatrix[1][1], m_destinationMatrix[1][2]);
	const float totalAngle = glm::angle(currUp, destUp);
	const float maxAngleInStep = atan2f(stepDistanceLeft, m_distanceToFurthestParticle);
	const float angleChange = min(totalAngle, maxAngleInStep);
	if (angleChange > Configuration.GLASS_ANGLE_PRECISION) {
		const glm::vec3 perpVec = getPerpendicular(currUp, destUp);
		m_matrix = glm::rotate(m_matrix, angleChange, perpVec);
	}

	stepDistanceLeft -= tanf(angleChange) * m_distanceToFurthestParticle;
	stepDistanceChange = min(distance - stepDistanceChange, stepDistanceLeft);
	if (distance > Configuration.GLASS_DISTANCE_PRECISION) {
		const glm::vec3 stepVec = glm::normalize(difVec) * stepDistanceChange;
		//m_matrix = glm::translate(m_matrix, stepVec);
		m_matrix[3][0] += stepVec.x;
		m_matrix[3][1] += stepVec.y;
		m_matrix[3][2] += stepVec.z;
		stepDistanceLeft -= stepDistanceChange;
	}

}
