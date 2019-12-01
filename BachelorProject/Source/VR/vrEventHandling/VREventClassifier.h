#pragma once

#include <OpenVR/openvr.h>

#include <VR/vrEventHandling/VREventClassifiedType.h>
#include <VR/vrDataProviders/Provider.h>

namespace VR
{
	namespace EventHandling
	{
		namespace VREventClassifier
		{
			VREventClassifiedType ClassifyEvent(vr::VREvent_t VrEvent);
			VR::ProvidedDataTypes::ProvidedData* GenerateData(vr::VREvent_t VrEvent, VREventClassifiedType Type);
		}
	}
}
