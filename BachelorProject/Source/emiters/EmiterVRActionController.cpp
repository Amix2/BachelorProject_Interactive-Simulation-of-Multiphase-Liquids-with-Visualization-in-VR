#include "EmiterVRActionController.h"
enum class RespectOffset {
	NONE, ONLY_ANGLE, ALL
};

RespectOffset m_respectOffset = RespectOffset::NONE;

void EmiterVRActionController::triggerButton(vr::InputDigitalActionData_t event) const
{



	if(event.bActive and event.bChanged)
		m_respectOffset = RespectOffset( ((int)m_respectOffset+1) % 3);

}

void EmiterVRActionController::gripButton(vr::InputDigitalActionData_t event) const
{
	this->m_emiter->m_updateMatrix = event.bState;
}

void EmiterVRActionController::menuButton(vr::InputDigitalActionData_t event) const
{
}


void EmiterVRActionController::touchpadMovement(const glm::vec2& position, const glm::vec2& move) const
{
	if (move.x < 0)
	{
		this->m_emiter->decreaseVelocity(-10 * move.x);
	}
	else
	{
		this->m_emiter->increaseVelocity(10 * move.x);
	}
	if (move.y < 0)
	{
		this->m_emiter->decreaseSize(-10 * move.y);
	}
	else
	{
		this->m_emiter->increaseSize(10 * move.y);
	}

}

void EmiterVRActionController::handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) const
{
	switch(m_respectOffset) {
	case RespectOffset::NONE:
		m_emiter->updateMatrix(positionMatrix);
		break;
	case RespectOffset::ONLY_ANGLE:
		glm::mat4 offset = grabOffset;
		Utils::setPosition(&offset, { 0,0,0 });
		m_emiter->updateMatrix(positionMatrix * offset);
		break;
	case RespectOffset::ALL:
		m_emiter->updateMatrix(positionMatrix * grabOffset);
		break;

	}
}
