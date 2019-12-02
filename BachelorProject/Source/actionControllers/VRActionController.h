#pragma once
#include <actionControllers/ActionController.h>
#include <OpenVR/openvr.h>


class VRActionController : public ActionController {
public:

	virtual void triggerButton(vr::InputDigitalActionData_t event) const { defaultAction("triggerButton"); }
	virtual void gripButton(vr::InputDigitalActionData_t event) const { defaultAction("gripButton"); }
	virtual void menuButton(vr::InputDigitalActionData_t event) const { defaultAction("menuButton"); }

	virtual void touchpadMovement(const glm::vec2 &position, const glm::vec2 &move) const { defaultAction("touchpadMovement"); }

	virtual void handMovement(const glm::mat4 &position, const glm::mat4 &grabOffset) const { defaultAction("handMovement"); }
};