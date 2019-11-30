#include <OpenVR/openvr.h>

#include "VRInput.h"

namespace VR
{
	namespace Implementation
	{
		std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> VRInput::GetDetectedControllers()
		{
			return this->DetectedControllers;
		}

		std::vector<vr::VREvent_t> VRInput::GetDetectedEvents(vr::ETrackedControllerRole ControllerRole)
		{
			return this->DetectedEvents;
		}

		bool VRInput::ControllersAlreadyDetected()
		{
			if (this->DetectedControllers.first == vr::k_unTrackedDeviceIndexInvalid || this->DetectedControllers.second == vr::k_unTrackedDeviceIndexInvalid)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool VRInput::InitModule()
		{
			return true;
		}

		bool VRInput::DetectControllers()
		{
			std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> DetectedControllers{ vr::k_unTrackedDeviceIndexInvalid, vr::k_unTrackedDeviceIndexInvalid };
			for (unsigned int TrackedDeviceIndex = 0; TrackedDeviceIndex < vr::k_unMaxTrackedDeviceCount; ++TrackedDeviceIndex) {
				vr::ETrackedDeviceClass TrackedDeviceClass = this->VrCore->GetVrSystem()->GetTrackedDeviceClass(TrackedDeviceIndex);
				if (TrackedDeviceClass != vr::ETrackedDeviceClass::TrackedDeviceClass_Controller || !VrCore->GetVrSystem()->IsTrackedDeviceConnected(id))
				{
					continue;
				}
				else
				{
					vr::ETrackedControllerRole TrackedControllerRole = this->VrCore->GetVrSystem()->GetControllerRoleForTrackedDeviceIndex(TrackedDeviceIndex);
					if (TrackedControllerRole == vr::TrackedControllerRole_LeftHand)
					{
						this->DetectedControllers.first = TrackedDeviceIndex;
					}
					else if (TrackedControllerRole == vr::TrackedControllerRole_RightHand)
					{
						this->DetectedControllers.second = TrackedDeviceIndex;
					}
				}

				//vr::TrackedDevicePose_t trackedDevicePose;
				//vr::VRControllerState_t controllerState;
				//VRCore->GetVrSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, id, &controllerState, sizeof(controllerState), &trackedDevicePose);
			}

			this->DetectedControllers = DetectedControllers;

			return true;
		}

		bool VRInput::DetectEvents(vr::ETrackedControllerRole ControllerRole)
		{
			vr::VREvent_t VrEvent;
			std::vector<vr::VREvent_t> DetectedEvents;
			while (this->VrCore->GetVrSystem()->PollNextEvent(&VrEvent, sizeof(VrEvent)))
			{
				DetectedEvents.push_back(VrEvent);
				//switch (VrEvent.eventType)
				//{
				//case vr::VREvent_ButtonPress:

				//case vr::VREvent_ButtonTouch:

				//case vr::VREvent_ButtonUnpress:

				//case vr::VREvent_ButtonUntouch:
				//	break;
				//}
			}
			this->DetectedEvents = DetectedEvents;
			if (this->DetectedEvents.size() == 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
}
