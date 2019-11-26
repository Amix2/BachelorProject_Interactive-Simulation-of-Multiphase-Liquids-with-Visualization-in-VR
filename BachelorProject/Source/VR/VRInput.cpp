/// Corresponding header inclusion
#include "VRInput.h"

namespace VR {
	bool VRInput::init()
	{
		//
	}

	std::pair<vr::ETrackedControllerRole, vr::ETrackedControllerRole> VRInput::GetDetectedControllers()
	{
		return this->DetectedControllers;
	}

	std::vector<std::pair<vr::EVREventType, vr::EVRButtonId>> VRInput::GetDetectedEvents(vr::ETrackedControllerRole ControllerRole)
	{
		if (this->DetectedControllers.first == ControllerRole)
		{
			return this->DetectedEvents.first;
		}
		else if (this->DetectedControllers.second == ControllerRole)
		{
			return this->DetectedEvents.second;
		}
	}

	bool VRInput::DetectControllers()
	{
		std::pair<vr::ETrackedControllerRole, vr::ETrackedControllerRole> DetectedControllers{ 0, 0 };
		for (unsigned int id = 0; id < vr::k_unMaxTrackedDeviceCount; ++id) {
			vr::ETrackedDeviceClass trackedDeviceClass = this->VRCore->GetVrSystem()->GetTrackedDeviceClass(id);
			if (trackedDeviceClass != vr::ETrackedDeviceClass::TrackedDeviceClass_Controller || !VRCore->GetVrSystem()->IsTrackedDeviceConnected(id))
			{
				continue;
			}
			else
			{
				vr::ETrackedControllerRole TrackedControllerRole = this->VrCore->GetVRSystem()->GetControllerRoleForTrackedDeviceIndex(event.trackedDeviceIndex);
				if (this-)
				{
					this->DetectedControllers.first = 
				}
			}

			//Confirmed that the device in question is a connected controller

			//This is all copied from above:
			vr::TrackedDevicePose_t trackedDevicePose;
			vr::VRControllerState_t controllerState;
			VRCore->GetVrSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, id, &controllerState, sizeof(controllerState), &trackedDevicePose);

			//Do things with the TrackedDevicePose_t struct
		}
		if ()

		this->DetectedControllers = DetectedControllers;

		return true;
	}

	bool VRInput::DetectEvents(vr::ETrackedControllerRole ControllerRole)
	{
		vr::VREvent_t VrEvent;
		std::pair<vr::ETrackedControllerRole, std::vector<std::pair<vr::EVREventType, vr::EVRButtonId>>> DetectedEvents;
		while (this->VRCore->GetVrSystem()->PollNextEvent(&VrEvent, sizeof(VrEvent)))
		{
			switch (VrEvent.eventType)
			{
			case vr::VREvent_ButtonPress:

			case vr::VREvent_ButtonTouch:

			case vr::VREvent_ButtonUnpress:

			case vr::VREvent_ButtonUntouch:
				break;
			}
		}
		this->DetectedEvents = DetectedEvents;
		if (this->DetectedEvents.second.size() == 0)
		{
			return false;
		}
		else {
			return true;
		}
	}
}
