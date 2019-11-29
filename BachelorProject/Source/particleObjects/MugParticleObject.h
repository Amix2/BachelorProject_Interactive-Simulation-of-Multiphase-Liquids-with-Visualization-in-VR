#pragma once
#include <particleObjects/ParticleObject.h>
#include <Utils.h>


class MugParticleObject :
	public ParticleObject
{
	inline static const float MUG_TURN_VELOCITY_CHANGE = 1.f;	// max position change increase in 1 turn comparing to previous turn
	float previousTurnPositionChange = 0;

	void create(ParticleObjectDetais details, int& numOfParts);
	float stepPositionChange(const float maxDistance);
	float stepAngleChange(const float maxDistance);
public:
	bool isSelected() const override { return m_selected; }
	void grab() override;
	void release() override;
	glm::mat4* getMatrix() override;

	void setDestinationMatrix(const glm::mat4& matrix) { m_destinationMatrix = matrix; }


	glm::mat4 getHelp() { return m_destinationMatrix; }

	MugParticleObject(ParticleObjectDetais details, int& numOfParts);

	void stepTowardsDestination() override;

};

