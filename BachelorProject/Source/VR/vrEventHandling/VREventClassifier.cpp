#include <memory>

#include <VR/VRInput.h>
#include <VR/vrDataProviders/vrProvidedData/ControllerInputData.h>

#include "VREventClassifier.h"

namespace VR
{
	namespace EventHandling
	{
		namespace VREventClassifier
		{
			VR::EventHandling::VREventClassifiedType ClassifyEvent(vr::VREvent_t VrEvent, std::shared_ptr<VR::Implementation::VRInput> VrInput)
			{
				VREventClassifiedType Type;
				switch (VrEvent.eventType)
				{
				case vr::VREvent_ButtonPress:
				case vr::VREvent_ButtonUnpress:
					if (VrEvent.trackedDeviceIndex == VrInput->GetDetectedControllers().first)
					{
						Type = VREventClassifiedType::VREVENT_BUTTON_LEFT;
					}
					else if (VrEvent.trackedDeviceIndex == VrInput->GetDetectedControllers().second)
					{
						Type = VREventClassifiedType::VREVENT_BUTTON_RIGHT;
					}
					break;

				case vr::VREvent_ButtonTouch:
				case vr::VREvent_ButtonUntouch:
					if (VrEvent.trackedDeviceIndex == VrInput->GetDetectedControllers().first)
					{
						Type = VREventClassifiedType::VREVENT_TOUCHPAD_LEFT;
					}
					else if (VrEvent.trackedDeviceIndex == VrInput->GetDetectedControllers().second)
					{
						Type = VREventClassifiedType::VREVENT_TOUCHPAD_RIGHT;
					}
					break;

				default:
					Type = VREventClassifiedType::VREVENT_INVALID;
				}

				return Type;
			}

			VR::ProvidedDataTypes::ProvidedData* GenerateData(vr::VREvent_t VrEvent, VREventClassifiedType Type)
			{
				VR::ProvidedDataTypes::ProvidedData* GeneratedData;
				switch (Type)
				{
				case VREventClassifiedType::VREVENT_BUTTON:
					GeneratedData = new VR::ProvidedDataTypes::ControllerInputData();
					break;

				case VREventClassifiedType::VREVENT_TOUCHPAD:
					GeneratedData = new VR::ProvidedDataTypes::ControllerInputData();
					break;

				case VREventClassifiedType::VREVENT_INVALID:
					GeneratedData = nullptr;
					break;
				}

				return GeneratedData;
			}
		}
	}
}
