#pragma once
#include <particleObjects/ParticleObject.h>


class MugParticleObject :
	public ParticleObject
{
	inline static const float MUG_TURN_VELOCITY_CHANGE = 1.f;	// max position change increase in 1 turn comparing to previous turn
	float previousTurnPositionChange = 0;
public:
	MugParticleObject(ParticleObjectDetais details, int& numOfParts);

	void stepTowardsDestination() override;
};

