#pragma once

#include <map>
#include <string>
#include <vector>
#include <array>

#include <OpenVR/openvr.h>

#include <memory>

#include "Utils.h"

namespace VR {
	enum class InputButtons {
		MENU_BUTTON,
		GRIP_BUTTON,
		REAR_HANDLE,
	};

	enum class InputTouchpadAxes {
		//
	};

	enum class ControllerType {
		CONTROLLER_LEFT,
		CONTROLLER_RIGHT,
		HMD
	};

	class ActionLoader {
	public:
		ActionLoader(std::string PathToActionsFile);
		vr::VRActionHandle_t GetActionHandle(std::string Name);
		vr::VRActionSetHandle_t GetActionSet();

	protected:
		//

	private:
		bool InitActionsMap();

		std::map<std::string, vr::VRActionHandle_t> ActionMap;
		std::map<std::string, vr::VRInputValueHandle_t> ActionInputSourceMap;
		vr::VRActionSetHandle_t ActionSet = vr::k_ulInvalidActionSetHandle;
		std::string PathToActionsFile;
	};

	class VRInput {
	public:
		bool InitializeVRInput(std::string PathToActions);
		std::vector<std::string> DetectPressedButtons();
		bool HandleInput();

	protected:
		//

	private:
		std::unique_ptr<ActionLoader> ActionLoader;
		vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
		vr::InputAnalogActionData_t AnalogData;
		std::array<float, 2> AnalogDataArray;

		bool GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
		bool GetDigitalActionState(vr::VRActionHandle_t ActionHandle, vr::VRInputValueHandle_t* DevicePath = nullptr);
	};
}
