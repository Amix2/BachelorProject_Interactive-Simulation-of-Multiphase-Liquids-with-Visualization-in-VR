#pragma once

#include <map>
#include <string>
#include <vector>
#include <array>
#include <utility>

#include <OpenVR/openvr.h>

#include <VR/VRCore.h>

#include <memory>

#include "Utils.h"

namespace VR {
	class VRInput {
	public:
		VRInput(VRCore* VrCore);
		//bool InitializeVRInput(std::string PathToActions);
		std::vector<std::string> DetectPressedButtons();
		//bool HandleInput();
		bool ControllersAlreadyDetected();
		bool DetectControllers();
		std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> GetDetectedControllers();

	protected:
		//

	private:
		VRCore* VrCore;
		std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> DetectedControllers{ vr::k_unMaxTrackedDeviceCount, vr::k_unMaxTrackedDeviceCount };
		vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
		vr::InputAnalogActionData_t AnalogData;
		std::array<float, 2> AnalogDataArray;

		//bool GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
		//bool GetDigitalActionState(vr::VRActionHandle_t ActionHandle, vr::VRInputValueHandle_t* DevicePath = nullptr);
	};
}
