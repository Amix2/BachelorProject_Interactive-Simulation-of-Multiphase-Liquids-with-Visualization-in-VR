#pragma once
#include <actionControllers/VRActionController.h>
#include <particleObjects/ParticleObject.h>
class ParticleObject;

class ParticleObjectVRActionController : public VRActionController{
public:
	ParticleObjectVRActionController(ParticleObject* object) : VRActionController(), m_object{ object } {}

	void triggerButton(vr::InputDigitalActionData_t event) override;
	void gripButton(vr::InputDigitalActionData_t event) override;
	//void menuButton() const { defaultAction("menuButton"); }
	//void touchpadMovement(const glm::vec2& position, const glm::vec2& move) const { defaultAction("touchpadMovement"); }
	void handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) override;

private:
	ParticleObject* m_object;
	bool isGrabbed = false;
	bool isPulled = false;
};