#pragma once
#include <particleObjects/ParticleObject.h>


class MugParticleObject :
	public ParticleObject
{
public:
	MugParticleObject(ParticleObjectDetais details, int& numOfParts);

	void stepTowardsDestination();
};

