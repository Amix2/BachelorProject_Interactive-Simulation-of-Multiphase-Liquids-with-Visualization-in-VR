#include "ParticleObject.h"


void ParticleObject::createMug(ParticleObjectDetais details, float positions[], float vectors[], int& numOfParts)
{
	const float inCircleGap = Configuration.GLASS_PARTICLE_BUILD_GAP; // gap on oX, in a circle
	const float layerGap = (float)inCircleGap * (float)sqrt(3) / 2;	// gap on oY
	const float innerRadius = details.innerRadius;
	const glm::vec3 localCenter = glm::vec3(0,0,0);
	const glm::vec3 objectCenter = glm::vec3(details.cX, details.cY, details.cZ);
	const float thickness = details.thickness;
	const float height = details.height;
	const float top = localCenter.y + height / 2;
	const glm::vec3 topVec = localCenter + glm::vec3(0, height / 2, 0);
	const float bottom = localCenter.y - height / 2;
	const glm::vec3 bottomVec = localCenter - glm::vec3(0, height / 2, 0);
	const glm::vec3 layerGapVecY = glm::vec3(0, layerGap, 0);

	// set up attributes
	m_currentPosition = objectCenter;
	m_targetPosition = m_currentPosition;
	m_indBegin = ParticleData::m_GlassParticlesNum;
	m_currentVector = glm::vec3(0, 1, 0);
	m_targetVector = m_currentVector;
	//	m_endInd is assigned in the end


	// x->cylinders || v->filled Circles
	// are drown allways from the right side towards middle (2nd condition: from top to bottom looking at blob of the same type)
	if (thickness <= layerGap) {
		/*
				x x         x x
				x x         x x
				x x  v v v  x x
				x x  v v v  x x
		*/

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top		// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);	


		ParticleGeometry::filledCircle(positions, vectors, numOfParts
			, bottomVec
			// calculate particles for range (0 ,  innerRadius), render for range [0 , innerRadius-layerGap/2)
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);	

		ParticleGeometry::filledCircle(positions, vectors, numOfParts
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

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 2 * layerGap, top - layerGap / 2		// heights
			, innerRadius + 2 * layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 2 * layerGap, top 	// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::ZERO);	// always should be here

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top - layerGap / 2	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here


		ParticleGeometry::filledCircle(positions, vectors, numOfParts
			, bottomVec
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		ParticleGeometry::filledCircle(positions, vectors, numOfParts
			, bottomVec - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::ZERO);	// always should be here

		ParticleGeometry::filledCircle(positions, vectors, numOfParts
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

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 3 * layerGap, top - layerGap		// heights
			, innerRadius + 3 * layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - 3 * layerGap, top 	// heights
			, innerRadius + 2 * layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::OUTSIDE);	// always should be here

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top 	// heights
			, innerRadius + layerGap	// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here

		ParticleGeometry::cylinder(positions, vectors, numOfParts
			, glm::vec2(localCenter.x, localCenter.z)
			, bottom - layerGap, top - layerGap	// heights
			, innerRadius				// radius
			, inCircleGap, layerGap, ParticleGeometry::INSIDE);	// always should be here


		ParticleGeometry::filledCircle(positions, vectors, numOfParts
			, bottomVec
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		ParticleGeometry::filledCircle(positions, vectors, numOfParts
			, bottomVec - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius, innerRadius - layerGap / 2, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::UP);	// always should be here

		ParticleGeometry::filledCircle(positions, vectors, numOfParts
			, bottomVec - layerGapVecY - layerGapVecY
			, 0, 0, ParticleGeometry::EQUALS		// inner
			, innerRadius + 2 * layerGap, innerRadius + 1.5 * layerGap, ParticleGeometry::NOT_EQUALS	// outer
			, inCircleGap, layerGap, ParticleGeometry::DOWN);	// always should be here

		ParticleGeometry::filledCircle(positions, vectors, numOfParts
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


	m_indEnd = ParticleData::m_GlassParticlesNum + numOfParts;
}

std::string ParticleObject::print()
{
	std::ostringstream stringStream;
	stringStream 
		<< "Position ( " << m_currentPosition.x << " " << m_currentPosition.y << " " << m_currentPosition.z 
		<< " ) => ( "		<< m_targetPosition.x << " " << m_targetPosition.y << " " << m_targetPosition.z 
		<< " ), Vector [ "	<< m_currentVector.x << " " << m_currentVector.y << " " << m_currentVector.z 
		<< " ] = > [ "	<< m_targetVector.x << " " << m_targetVector.y << " " << m_targetVector.z 
		<< " ] , In glass particle array [ " << m_indBegin << " " << m_indEnd << " )";
	return  stringStream.str();
}

ParticleObject::ParticleObject() {
	m_currentPosition = glm::vec3(0,0,0);
	m_currentVector = glm::vec3(0, 0, 0);
	m_indBegin = 0;
	m_indEnd = 0;
	m_targetPosition = glm::vec3(0, 0, 0);
	m_targetVector = glm::vec3(0, 0, 0);
}