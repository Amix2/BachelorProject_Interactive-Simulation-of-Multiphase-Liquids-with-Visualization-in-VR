#pragma once

#include <map>
#include <string>
#include <vector>

#include <OpenVR/openvr.h>

#include <memory>

#include <VR/vrDataProviders/ProvidedData.h>

#include "VRCore.h"

#include "Utils.h"

namespace VR {
	/*
	class ActionLoader {
	public:
		// ActionLoader(std::string PathToActionsFile);
		// vr::VRActionHandle_t GetActionHandle(std::string Name);
		// vr::VRActionSetHandle_t GetActionSet();

	protected:
		//

	private:
		bool InitActionsMap();

		// std::map<std::string, vr::VRActionHandle_t> ActionMap;
		// std::map<std::string, vr::VRInputValueHandle_t> ActionInputSourceMap;
		// vr::VRActionSetHandle_t ActionSet = vr::k_ulInvalidActionSetHandle;
		// std::string PathToActionsFile;
	};

	class VRInput {
	public:
		bool InitializeVRInput(std::string PathToActions);
		std::vector<std::string> DetectPressedButtons();
		bool HandleInput();

	protected:
		//

	private:
		// std::unique_ptr<ActionLoader> ActionLoader;
		vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
		// vr::InputAnalogActionData_t AnalogData;
		// std::array<float, 2> AnalogDataArray;

		// bool GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
		// bool GetDigitalActionState(vr::VRActionHandle_t ActionHandle, vr::VRInputValueHandle_t* DevicePath = nullptr);
	};
	*/
	struct CompleteVRControllerEvent : DataProviders::ProvidedData
	{
		vr::EVREventType EventType;
		vr::EVRButtonId ButtonId;
	};

	class VRInput
	{
	public: 
		VRInput(const std::shared_ptr<VRCore> VRCore);
		bool init();
		bool DetectControllers();
		bool DetectEvents();
		std::pair<vr::ETrackedControllerRole, vr::ETrackedControllerRole> GetDetectedControllers();
		std::vector<CompleteVRControllerEvent> GetDetectedEvents();

	protected:
		//

	private:
		std::shared_ptr<VRCore> VRCore;
		vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
		std::pair<vr::ETrackedControllerRole, vr::ETrackedControllerRole> DetectedControllers;
		std::vector<CompleteVRControllerEvent> DetectedEvents;
	};
}
