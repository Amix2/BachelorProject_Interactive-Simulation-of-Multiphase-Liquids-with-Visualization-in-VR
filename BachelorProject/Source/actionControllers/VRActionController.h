#pragma once
#include <actionControllers/ActionController.h>


class VRActionController : public ActionController {
public:

	virtual void triggerButton() const { defaultAction("triggerButton"); }
	virtual void gripButton() const { defaultAction("gripButton"); }
	virtual void menuButton() const { defaultAction("menuButton"); }
	virtual void touchpadMovement(const glm::vec2 &position, const glm::vec2 &move) const { defaultAction("touchpadMovement"); }
	virtual void handMovement(const glm::mat4 &position, const glm::mat4 &grabOffset) const { defaultAction("handMovement"); }
};