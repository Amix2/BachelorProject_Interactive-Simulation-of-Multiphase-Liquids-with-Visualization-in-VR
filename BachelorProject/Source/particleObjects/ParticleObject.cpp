#include "ParticleObject.h"

void ParticleObject::rotateX(float angle)
{
}

void ParticleObject::rotateY(float angle)
{
}

void ParticleObject::rotateZ(float angle)
{
}

MugObject::MugObject(ParticleObjectDetais details, float positions[], int& numOfParts)
{
	const float inCircleGap = Configuration.GLASS_PARTICLE_BUILD_GAP; // gap on oX, in a circle
	const float layerGap = inCircleGap * sqrt(3) / 2;	// gap on oY
	const float innerRadius = details.innerRadius;
	const glm::vec3 center = glm::vec3(details.cX, details.cY, details.cZ);
	const float thickness = details.thickness;
	const float height = details.height;
	const float top = center.y + height / 2;
	const glm::vec3 topVec = center + glm::vec3(0, height / 2, 0);
	const float bottom = center.y - height / 2;
	const glm::vec3 bottomVec = center - glm::vec3(0, height / 2, 0);
	const glm::vec3 layerGapVecY = glm::vec3(0, layerGap, 0);
	LOG_F(INFO, "layerGap %f", layerGap);

	// x->cylinders || v->filled Circles
	// are drawn allways from the right side towards middle (2nd condition: from top to bottom looking at blob of the same type)
	if (thickness <= layerGap) {
		/*
				x x         x x
				x x         x x
				x x  v v v  x x
				x x  v v v  x x
		*/

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - layerGap, top		// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - layerGap, top	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap);	// always should be here


		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec
			// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec - layerGapVecY
			// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

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

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - 2*layerGap, top-layerGap/2		// heights
			, innerRadius + 2*layerGap	// radius
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - 2 * layerGap, top 	// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - layerGap, top - layerGap/2	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap);	// always should be here


		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec - layerGapVecY
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius+layerGap, innerRadius + layerGap / 2, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here
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

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - 3 * layerGap, top - layerGap		// heights
			, innerRadius + 3 * layerGap	// radius
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - 3 * layerGap, top 	// heights
			, innerRadius + 2 * layerGap	// radius
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - layerGap, top 	// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, bottom - layerGap, top - layerGap	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap);	// always should be here


		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec - layerGapVecY
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius + 2*layerGap, innerRadius + 1.5 * layerGap, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

		ParticleGeopetry::filledCircle(positions, numOfParts
			, bottomVec - layerGapVecY - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeopetry::EQUALS		// inner
			, innerRadius + 2 * layerGap, innerRadius + 1.5 * layerGap, ParticleGeopetry::NOT_EQUALS	// outer
			, inCircleGap, layerGap);	// always should be here

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



}
	