#pragma once
#include <particleObjects/ParticleObject.h>


class MugParticleObject :
	public ParticleObject
{
	inline static const float MUG_TURN_VELOCITY_CHANGE = 1.f;	// max position change increase in 1 turn comparing to previous turn
	float previousTurnPositionChange = 0;

	void create(ParticleObjectDetais details, int& numOfParts);
public:
	bool isSelected() const override { return m_selected; }
	void grab() override;
	void release() override;
	glm::mat4* getMatrix() override;
	void setMatrix(const glm::mat4& matrix) override;


	MugParticleObject(ParticleObjectDetais details, int& numOfParts);

	void stepTowardsDestination() override;

};

