#include "EmiterVRActionController.h"

void EmiterVRActionController::triggerButton(vr::InputDigitalActionData_t event) const
{
	//
}

void EmiterVRActionController::gripButton(vr::InputDigitalActionData_t event) const
{
	this->m_emiter->m_updateMatrix = event.bState;
}

void EmiterVRActionController::menuButton(vr::InputDigitalActionData_t event) const
{
}

void EmiterVRActionController::touchpad(vr::InputAnalogActionData_t event) const
{
	if (event.deltaX < 0)
	{
		this->m_emiter->decreaseVelocity(-10 * event.deltaX);
	}
	else
	{
		this->m_emiter->increaseVelocity(10 * event.deltaX);
	}
	if (event.deltaY < 0)
	{
		this->m_emiter->decreaseSize(-10 * event.deltaY);
	}
	else
	{
		this->m_emiter->increaseSize(10 * event.deltaY);
	}
}

void EmiterVRActionController::handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) const
{
	m_emiter->updateMatrix(positionMatrix);
}
