#pragma once

#include <OpenVR/openvr.h>
#include <Logger.h>

class SteamIVRInput {
public:
	void Init();
	void InnerActionUpdate();
	bool nextSongSet();
	bool GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
	bool GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);

	vr::VRActionHandle_t m_actionTrigger = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionApplicationMenu = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionGrip = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionAnalongInput = vr::k_ulInvalidActionHandle;

	vr::VRActionSetHandle_t m_actionsetDemo = vr::k_ulInvalidActionSetHandle;

	struct ControllerInfo_t
	{
		vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionHaptic = vr::k_ulInvalidActionHandle;
		std::string m_sRenderModelName;
		bool m_bShowController;
	};

	enum EHand
	{
		Left = 0,
		Right = 1,
	};
	ControllerInfo_t m_rHand[2];

private:
	//Using an action manifest from a path that isn't the executable path doesn't work for whatever reason.
	const std::string m_actionManifestPath = "C:\\Users\\admin\\Desktop\\cmder\\BachelorProject\\x64\\Debug\\action_manifest.json";
	vr::VRActionHandle_t m_nextSongHandler = {};
	vr::VRActionHandle_t m_RightTriggerButtonHandler = {};
	vr::VRActionHandle_t m_LeftTriggerButtonHandler = {};
	vr::VRActionHandle_t m_RightGripButtonHandler = {};
	vr::VRActionHandle_t m_LeftGripButtonHandler = {};
	vr::VRActionSetHandle_t m_mainSetHandler = {};
	vr::VRActiveActionSet_t m_activeActionSet = {};
	vr::InputDigitalActionData_t m_nextSongData = {};
};

// These two are set in action_manifest.json. They must match or you'll get errors/things that don't work with no errors.
constexpr auto k_actionSetMain = "/actions/main";
constexpr auto k_actionPlayNextTrack = "/actions/main/in/PlayNextTrack";

