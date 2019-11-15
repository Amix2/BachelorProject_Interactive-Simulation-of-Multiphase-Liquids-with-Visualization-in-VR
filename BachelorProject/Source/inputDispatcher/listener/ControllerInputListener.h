#pragma once

#include <OpenVR/openvr.h>

class ControllerInputListener
{
public:
	virtual void handleControllerInput(const vr::EVREventType eventType, const vr::EVRButtonId buttonId) = 0;
};