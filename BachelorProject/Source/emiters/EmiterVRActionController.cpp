#include "EmiterVRActionController.h"
enum class RespectOffset {
	ALL
};

RespectOffset m_respectOffset = RespectOffset::ALL;

void EmiterVRActionController::onRelease()
{
	m_emiter->toggleActive();
}

void EmiterVRActionController::triggerButton(vr::InputDigitalActionData_t event)
{
	if (event.bState and event.bChanged) {
		m_emiter->toggleActive();
	}
		
}

void EmiterVRActionController::gripButton(vr::InputDigitalActionData_t event) 
{
	this->m_emiter->m_updateMatrix = event.bState;
}

void EmiterVRActionController::menuButton(vr::InputDigitalActionData_t event) 
{
}

void EmiterVRActionController::touchpadButton(vr::InputDigitalActionData_t event)
{
	if (event.bState and event.bChanged) {
		//LOG_F(WARNING, "press %f, %f", m_lastTouchpadPosition.x, m_lastTouchpadPosition.y);
		auto x = m_lastTouchpadPosition.x;
		auto y = m_lastTouchpadPosition.y;
		if (y > x and y > -x) // 1
		{
			this->m_emiter->increaseVelocity();
		}
		else if (y < x and y > -x) // 2
		{
			this->m_emiter->increaseSize(1);
		}
		else if (y < x and y < -x) // 3
		{
			this->m_emiter->decreaseVelocity();
		}
		else if (y < -x and y > x) // 4
		{
			this->m_emiter->decreaseSize(1);
		}
	}
}


void EmiterVRActionController::touchpadMovement(const glm::vec2& position, const glm::vec2& move) 
{
	m_lastTouchpadPosition = position;
	m_lastTouchpadMove = move;
}

void EmiterVRActionController::handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) 
{
	switch(m_respectOffset) {
	case RespectOffset::ALL:
		m_emiter->updateMatrix(positionMatrix * grabOffset);
		break;

	}
}
