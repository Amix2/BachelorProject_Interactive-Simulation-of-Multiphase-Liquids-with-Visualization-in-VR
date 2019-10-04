#include "ParticleObject.h"


void ParticleObject::createMug(ParticleObjectDetais details, int& numOfParts)
{
	const float inCircleGap = Configuration.GLASS_PARTICLE_BUILD_GAP; // gap on oX, in a circle
	const float layerGap = (float)inCircleGap * (float)sqrt(3) / 2;	// gap on oY
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

	// set up attributes
	m_matrix = glm::translate(glm::mat4(1.0f), objectCenter);


	// x->cylinders || v->filled Circles
	// are drown allways from the right side towards middle (2nd condition: from top to bottom looking at blob of the same type)
	if (thickness <= layerGap) {
		/*
				x x         x x
				x x         x x
				x x  v v v  x x
				x x  v v v  x x
		*/

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top		// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);


		ParticleGeometry::filledCircle(numOfParts
			, bottomVec
			// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY
			// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::DOWN);

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

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 2 * layerGap, top - layerGap / 2		// heights
			, innerRadius + 2 * layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 2 * layerGap, top 	// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::ZERO);	// always should be here

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top - layerGap / 2	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here


		ParticleGeometry::filledCircle(numOfParts
			, bottomVec
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::ZERO);	// always should be here

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius + layerGap, innerRadius + layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::DOWN);	// always should be here
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

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 3 * layerGap, top - layerGap		// heights
			, innerRadius + 3 * layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 3 * layerGap, top 	// heights
			, innerRadius + 2 * layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top 	// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here

		ParticleGeometry::cylinder(numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top - layerGap	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here


		ParticleGeometry::filledCircle(numOfParts
			, bottomVec
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius + 2 * layerGap, innerRadius + 1.5 * layerGap, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::DOWN);	// always should be here

		ParticleGeometry::filledCircle(numOfParts
			, bottomVec - layerGapVecY - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius + 2 * layerGap, innerRadius + 1.5 * layerGap, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::DOWN);	// always should be here

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


	//m_indEnd = ParticleData::m_GlassParticlesNum + numOfParts;
}




ParticleObject::ParticleObject() {
	m_matrix = glm::mat4(3.0f);
}