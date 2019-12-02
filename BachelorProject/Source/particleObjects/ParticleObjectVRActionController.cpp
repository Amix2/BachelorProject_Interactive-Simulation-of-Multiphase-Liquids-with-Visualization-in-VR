#include "ParticleObjectVRActionController.h"

void ParticleObjectVRActionController::triggerButton(vr::InputDigitalActionData_t event) {
	if (event.bState == true)
		isPulled = true;
	else 
		isPulled = false;
}

void ParticleObjectVRActionController::gripButton(vr::InputDigitalActionData_t event) {
	if (event.bState == true)
		isGrabbed = true;
	else
		isGrabbed = false;
}

void ParticleObjectVRActionController::handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) {
	if (isPulled)
		m_object->setDestinationMatrix(glm::translate(glm::mat4{ 1 }, Utils::getPosition(positionMatrix)));
	else if (isGrabbed)
		m_object->setDestinationMatrix(positionMatrix * grabOffset);
	else
		m_object->setDestinationMatrix(*m_object->getMatrix());
}
