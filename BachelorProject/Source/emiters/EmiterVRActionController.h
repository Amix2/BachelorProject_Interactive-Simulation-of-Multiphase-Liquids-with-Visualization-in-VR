#pragma once
#include <actionControllers/VRActionController.h>
#include <emiters/Emiter.h>
class Emiter;

class EmiterVRActionController : public VRActionController {
public:
	EmiterVRActionController(Emiter* object) : m_emiter{ object } {}

	void triggerButton(vr::InputDigitalActionData_t event) const;
	void gripButton(vr::InputDigitalActionData_t event) const;
	void menuButton(vr::InputDigitalActionData_t event) const;

	void handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset) const;

private:
	Emiter* m_emiter;
};