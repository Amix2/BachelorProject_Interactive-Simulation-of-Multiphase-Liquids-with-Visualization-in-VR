#pragma once
#include <actionControllers/VRActionController.h>
#include <particleObjects/ParticleObject.h>
class ParticleObject;

class ParticleObjectVRActionController : public VRActionController{
public:
	ParticleObjectVRActionController(ParticleObject* object) : VRActionController(), m_object{ object } {}

	//void triggerButton(vr::InputDigitalActionData_t event) const;
	//void gripButton(vr::InputDigitalActionData_t event) const;
	//void menuButton(vr::InputDigitalActionData_t event) const;

	void handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) const { m_object->setDestinationMatrix(positionMatrix * grabOffset); }

private:
	ParticleObject* m_object;
};