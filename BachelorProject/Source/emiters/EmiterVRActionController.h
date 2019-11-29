#pragma once
#include <actionControllers/VRActionController.h>
#include <emiters/Emiter.h>
class Emiter;

class EmiterVRActionController : public VRActionController {
public:
	EmiterVRActionController(Emiter* object) : m_object{ object } {}

	void triggerButton() const { defaultAction("triggerButton"); }
	void gripButton() const { defaultAction("gripButton"); }
	void menuButton() const { defaultAction("menuButton"); }
	void touchpadMovement(const glm::vec2& position, const glm::vec2& move) const { defaultAction("touchpadMovement"); }
	void handMovement(const glm::mat4& position, const glm::mat4& grabOffset) const { defaultAction("handMovement"); }

private:
	Emiter* m_object;
};