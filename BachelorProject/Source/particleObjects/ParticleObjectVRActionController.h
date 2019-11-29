#pragma once
#include <actionControllers/VRActionController.h>
#include <particleObjects/ParticleObject.h>
class ParticleObject;

class ParticleObjectVRActionController : public VRActionController{
public:
	ParticleObjectVRActionController(ParticleObject* object) : VRActionController(), m_object{ object } {}

	void triggerButton() const { defaultAction("triggerButton"); }
	void gripButton() const { defaultAction("gripButton"); }
	void menuButton() const { defaultAction("menuButton"); }
	void touchpadMovement(const glm::vec2& position, const glm::vec2& move) const { defaultAction("touchpadMovement"); }
	void handMovement(const glm::mat4& position, const glm::mat4& grabOffset) const { defaultAction("handMovement"); }

private:
	ParticleObject* m_object;
};