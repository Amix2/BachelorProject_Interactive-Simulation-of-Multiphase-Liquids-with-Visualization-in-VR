#include "EmiterVRActionController.h"

void EmiterVRActionController::triggerButton(vr::InputDigitalActionData_t event) const
{
}

void EmiterVRActionController::gripButton(vr::InputDigitalActionData_t event) const
{
}

void EmiterVRActionController::menuButton(vr::InputDigitalActionData_t event) const
{
}

void EmiterVRActionController::handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) const
{
	m_emiter->updateMatrix(positionMatrix);
}
