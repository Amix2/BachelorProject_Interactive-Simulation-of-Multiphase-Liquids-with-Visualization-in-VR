#pragma once
#include <actionControllers/VRActionController.h>
#include <emiters/Emiter.h>
class Emiter;

class EmiterVRActionController : public VRActionController {
public:
	EmiterVRActionController(Emiter* object) : m_emiter{ object } {}

	void onRelease() override;
	void triggerButton(vr::InputDigitalActionData_t event);
	void gripButton(vr::InputDigitalActionData_t event);
	void menuButton(vr::InputDigitalActionData_t event);
	void touchpadButton(vr::InputDigitalActionData_t event);

	void touchpadMovement(const glm::vec2& position, const glm::vec2& move);

	void handMovement(const glm::mat4& positionMatrix, const glm::mat4& grabOffset);

private:
	Emiter* m_emiter;
	bool setFluidType{ false };

	glm::vec2 m_lastTouchpadPosition;
	glm::vec2 m_lastTouchpadMove;
};
