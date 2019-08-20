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

MugObject::MugObject(ParticleObjectDetais details, float positions[Configuration.MAX_PARTICLES_ADDED_IN_TURN], int& numOfParts)
{
	const float inCircleGap = Configuration.GLASS_PARTICLE_BUILD_GAP; // gap on oX
	const float layerGap = inCircleGap * sqrt(3) / 2;	// gap on oY
	const float innerRadius = details.innerRadius;
	const glm::vec3 center = glm::vec3(details.cX, details.cY, details.cZ);
	const float thickness = details.thickness;
	const float height = details.height;
	LOG_F(INFO, "layerGap %f", layerGap);

	// inner inner wall
	ParticleGeopetry::cylinder(positions, numOfParts, glm::vec2(center.x, center.z), center.y - height/2, center.y + height/2 - layerGap, innerRadius, inCircleGap, layerGap);

	if (thickness > 2 * layerGap) {

		// outer inner wall
		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)
			, center.y - height / 2 - layerGap,		center.y + height / 2
			, innerRadius + layerGap
			, inCircleGap, layerGap);

		// outer outer wall
		ParticleGeopetry::cylinder(positions, numOfParts
			, glm::vec2(center.x, center.z)	//middle line
			, center.y - height / 2 - thickness + layerGap,		center.y + height / 2 - layerGap	// height from ($1) to ($2)
			, innerRadius + thickness	// radius
			, inCircleGap, layerGap);
	}
	else {
		if (thickness > layerGap) {
			// middle wall
			ParticleGeopetry::cylinder(positions, numOfParts
				, glm::vec2(center.x, center.z)	//middle line
				, center.y - height / 2 - thickness + layerGap, center.y + height / 2 - layerGap	// height from ($1) to ($2)
				, innerRadius + thickness/2	// radius
				, inCircleGap, layerGap);
		}
	}

	// inner outer wall
	ParticleGeopetry::cylinder(positions, numOfParts
		, glm::vec2(center.x, center.z)	//middle line
		, center.y - height / 2 - thickness,	center.y + height / 2	// height from ($1) to ($2)
		, innerRadius + thickness - layerGap	// radius
		, inCircleGap, layerGap);

	// top middle floor
	ParticleGeopetry::filledCircle(positions, numOfParts
		, center - glm::vec3(0, height / 2, 0)		// start in the center lowered by mugs inner height/2
		// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
		, 0,			0,						ParticleGeopetry::EQUALS		// inner
		, innerRadius,	innerRadius-layerGap/2,	ParticleGeopetry::NOT_EQUALS	// outer
		, inCircleGap, layerGap);	// gap between circles and gap inside a circle

	// bottom middle floor
	ParticleGeopetry::filledCircle(positions, numOfParts
		, center - glm::vec3(0, height / 2 + layerGap, 0)// start in the center lowered by mugs inner height/2 and by layerGap (1 layerGap lower than abowe)
		// calculate particles for range (0 ,  innerRadius + layerGap), render for range [0 , innerRadius + layerGap/2)
		, 0,						0,							ParticleGeopetry::EQUALS		// inner
		, innerRadius + layerGap,	innerRadius + layerGap/2,	ParticleGeopetry::NOT_EQUALS	// outer
		, inCircleGap, layerGap);	// gap between circles and gap inside a circle


	// top top floor
	ParticleGeopetry::filledCircle(positions, numOfParts
		, center + glm::vec3(0, height / 2, 0)// start in the center plus mugs inner height
		// calculate particles for range (innerRadius + layerGap ,  innerRadius + thickness - layerGap), render for range (innerRadius + 1.5*layerGap , innerRadius + thickness - 1.5*layerGap)
		, innerRadius + layerGap,				innerRadius + 1.5*layerGap,				ParticleGeopetry::NOT_EQUALS	// inner
		, innerRadius + thickness - layerGap,	innerRadius + thickness - 1.5*layerGap, ParticleGeopetry::NOT_EQUALS	// outer
		, inCircleGap, layerGap);	// gap between circles and gap inside a circle

	// bottom top floor
	ParticleGeopetry::filledCircle(positions, numOfParts
		, center + glm::vec3(0, height / 2 - layerGap, 0)// start in the center plus mugs inner height
		// calculate particles for range (innerRadius + layerGap ,  innerRadius + thickness - layerGap), render for range (innerRadius + 1.5*layerGap , innerRadius + thickness - 1.5*layerGap)
		, innerRadius + layerGap, innerRadius + 1.5 * layerGap, ParticleGeopetry::NOT_EQUALS	// inner
		, innerRadius + thickness - layerGap, innerRadius + thickness - 1.5 * layerGap, ParticleGeopetry::NOT_EQUALS	// outer
		, inCircleGap, layerGap);	// gap between circles and gap inside a circle

}
