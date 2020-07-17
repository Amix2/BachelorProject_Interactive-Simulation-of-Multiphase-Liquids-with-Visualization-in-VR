#include <filesystem>
#include <array>

#include "InputConfig.h"

#include "VRInput.h"

namespace VR {
	VRInput::VRInput(VRCore* VrCore) : VrCore{ VrCore }
	{
		//
	}

	bool VRInput::ControllersAlreadyDetected()
	{
		if (this->DetectedControllers.first == vr::k_unMaxTrackedDeviceCount || this->DetectedControllers.second == vr::k_unMaxTrackedDeviceCount)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool VRInput::DetectControllers()
	{
		std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> DetectedControllers{ vr::k_unTrackedDeviceIndexInvalid, vr::k_unTrackedDeviceIndexInvalid };
		for (unsigned int TrackedDeviceIndex = 0; TrackedDeviceIndex < vr::k_unMaxTrackedDeviceCount; ++TrackedDeviceIndex) {
			vr::ETrackedDeviceClass TrackedDeviceClass = this->VrCore->GetVrSystem()->GetTrackedDeviceClass(TrackedDeviceIndex);
			if (TrackedDeviceClass != vr::ETrackedDeviceClass::TrackedDeviceClass_Controller || !VrCore->GetVrSystem()->IsTrackedDeviceConnected(TrackedDeviceIndex))
			{
				continue;
			}
			else
			{
				vr::ETrackedControllerRole TrackedControllerRole = this->VrCore->GetVrSystem()->GetControllerRoleForTrackedDeviceIndex(TrackedDeviceIndex);
				if (TrackedControllerRole == vr::TrackedControllerRole_LeftHand)
				{
					DetectedControllers.first = TrackedDeviceIndex;
				}
				else if (TrackedControllerRole == vr::TrackedControllerRole_RightHand)
				{
					DetectedControllers.second = TrackedDeviceIndex;
				}
			}

			//vr::TrackedDevicePose_t trackedDevicePose;
			//vr::VRControllerState_t controllerState;
			//VRCore->GetVrSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, id, &controllerState, sizeof(controllerState), &trackedDevicePose);
		}

		this->DetectedControllers = DetectedControllers;

		return true;
	}

	std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> VRInput::GetDetectedControllers()
	{
		return this->DetectedControllers;
	}
}
