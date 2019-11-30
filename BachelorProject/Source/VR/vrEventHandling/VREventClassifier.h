#pragma once

#include <OpenVR/openvr.h>

#include <VR/vrEventHandling/VREventClassifiedType.h>

namespace VR
{
	namespace EventHandling
	{
		namespace VREventClassifier
		{
			VREventClassifiedType ClassifyEvent(vr::VREvent_t VrEvent);
		}
	}
}
