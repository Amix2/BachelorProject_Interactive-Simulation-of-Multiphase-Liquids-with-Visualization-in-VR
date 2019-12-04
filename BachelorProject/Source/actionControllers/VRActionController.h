#pragma once
#include <actionControllers/ActionController.h>
#include <OpenVR/openvr.h>


class VRActionController : public ActionController {
public:

	virtual void onRelease() { defaultAction("released"); }
	virtual void triggerButton(vr::InputDigitalActionData_t event) { defaultAction("triggerButton"); }
	virtual void gripButton(vr::InputDigitalActionData_t event) { defaultAction("gripButton"); }
	virtual void menuButton(vr::InputDigitalActionData_t event) { defaultAction("menuButton"); }

	virtual void touchpadMovement(const glm::vec2 &position, const glm::vec2 &move) { defaultAction("touchpadMovement"); }

	virtual void handMovement(const glm::mat4 &position, const glm::mat4 &grabOffset) { defaultAction("handMovement"); }
};