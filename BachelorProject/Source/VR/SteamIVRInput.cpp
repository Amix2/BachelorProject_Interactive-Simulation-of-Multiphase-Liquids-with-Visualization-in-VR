#include "SteamIVRInput.h"
#include <pathtools.h>
#include <iostream>
#include <OpenVR/openvr.h>

void SteamIVRInput::Init()
{

	vr::VRInput()->SetActionManifestPath("C:\\Users\\admin\\Desktop\\cmder\\BachelorProject\\x64\\Debug\\hellovr_actions.json");

	vr::VRInput()->GetActionHandle("/actions/demo/in/HideCubes", &m_actionTrigger);
	vr::VRInput()->GetActionHandle("/actions/demo/in/HideThisController", &m_actionApplicationMenu);
	vr::VRInput()->GetActionHandle("/actions/demo/in/TriggerHaptic", &m_actionGrip);
	vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_actionAnalongInput);

	vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);

	vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Left", &m_rHand[Left].m_actionHaptic);
	vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_rHand[Left].m_actionPose);

	vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Right", &m_rHand[Right].m_actionHaptic);
	vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);
	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_rHand[Right].m_actionPose);

}

bool SteamIVRInput::nextSongSet()
{
	auto e = vr::VRInput()->GetDigitalActionData(
		m_actionTrigger,
		&m_nextSongData,
		sizeof(m_nextSongData),
		vr::k_ulInvalidInputValueHandle);

	if (e != vr::EVRInputError::VRInputError_None)
	{
		// Print the rror code.
		std::cerr << e << '\n';
		std::cerr << "GetDigitalAction error.\n";
	}

	// Will basically just spam the console. To make sure it's visible even from a distance.
	if (m_nextSongData.bActive)
	{
		std::cout << "Action Set Active!\n";
	}

	return m_nextSongData.bState;
}

void SteamIVRInput::InnerActionUpdate()
{
	// Getting the correct sizeof is critical.
	// Make sure to match digital/analog with the function you're calling.
	this->m_activeActionSet.nPriority = 0;
	auto error = vr::VRInput()->UpdateActionState(
		&m_activeActionSet, sizeof(m_activeActionSet), 1);

	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Loop error.\n";
	}
}

bool SteamIVRInput::GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath)
{
	vr::InputDigitalActionData_t actionData;
	vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
	if (pDevicePath)
	{
		*pDevicePath = vr::k_ulInvalidInputValueHandle;
		if (actionData.bActive)
		{
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				*pDevicePath = originInfo.devicePath;
			}
		}
	}
	return actionData.bActive && actionData.bState;
}

bool SteamIVRInput::GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath)
{
	vr::InputDigitalActionData_t actionData;
	vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
	if (pDevicePath)
	{
		*pDevicePath = vr::k_ulInvalidInputValueHandle;
		if (actionData.bActive)
		{
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				*pDevicePath = originInfo.devicePath;
			}
		}
	}
	return actionData.bActive && actionData.bChanged && actionData.bState;
}
