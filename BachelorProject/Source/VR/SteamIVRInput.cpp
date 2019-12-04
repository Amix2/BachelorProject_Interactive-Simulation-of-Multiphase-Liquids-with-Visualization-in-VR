#include "SteamIVRInput.h"
#include <pathtools.h>
#include <iostream>
#include <utility>
#include <map>
#include <OpenVR/openvr.h>

void SteamIVRInput::Init()
{

	//auto error = vr::VRInput()->SetActionManifestPath("C:\\Users\\admin\\Desktop\\cmder\\BachelorProject\\x64\\Debug\\hellovr_actions.json");
	//if (error != vr::EVRInputError::VRInputError_None)
	//{
	//	std::cerr << "Action manifest error\n";
	//}

	//error = vr::VRInput()->GetActionHandle("/actions/demo/in/HideCubes", &m_actionTrigger);
	//if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }
	//error = vr::VRInput()->GetActionHandle("/actions/demo/in/HideThisController", &m_actionApplicationMenu); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }
	//error = vr::VRInput()->GetActionHandle("/actions/demo/in/TriggerHaptic", &m_actionGrip); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }
	//error = vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_actionAnalongInput); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }

	//error = vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }

	//error = vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Left", &m_rHand[Left].m_actionHaptic); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }
	//error = vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }
	//error = vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_rHand[Left].m_actionPose); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }

	//error = vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Right", &m_rHand[Right].m_actionHaptic); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }
	//error = vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }
	//error = vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_rHand[Right].m_actionPose); if (error != vr::EVRInputError::VRInputError_None) { std::cerr << "Handle error.\n"; }

	//return;

	//if (error != vr::EVRInputError::VRInputError_None){std::cerr << "Handle error.\n";}


	//////////////////////////////////////////////////////////////

		// Set the action manifest. This should be in the executable directory.
	// Defined by m_actionManifestPath.
	auto error = vr::VRInput()->SetActionManifestPath("C:\\Users\\admin\\Desktop\\cmder\\BachelorProject\\x64\\Debug\\action_manifest.json");
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Action manifest error\n";
	}

	// Get action handle
	error = vr::VRInput()->GetActionHandle("/actions/main/in/PlayNextTrack", &m_nextSongHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	///////////		TRIGGER
	error = vr::VRInput()->GetActionHandle("/actions/main/in/right_trigger_button", &m_RightTriggerButtonHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	error = vr::VRInput()->GetActionHandle("/actions/main/in/left_trigger_button", &m_LeftTriggerButtonHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	///////////		GRIP
	error = vr::VRInput()->GetActionHandle("/actions/main/in/right_grip_button", &m_RightGripButtonHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	error = vr::VRInput()->GetActionHandle("/actions/main/in/left_grip_button", &m_LeftGripButtonHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	///////////		ANALOG
	error = vr::VRInput()->GetActionHandle("/actions/main/in/right_analog", &m_RightAnalogHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	error = vr::VRInput()->GetActionHandle("/actions/main/in/left_analog", &m_LeftAnalogHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	///////////		MENU
	error = vr::VRInput()->GetActionHandle("/actions/main/in/right_menu_button", &m_RightMenuButtonHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}

	error = vr::VRInput()->GetActionHandle("/actions/main/in/left_menu_button", &m_LeftMenuButtonHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}




	// Get set handle
	error = vr::VRInput()->GetActionSetHandle("/actions/main", &m_mainSetHandler);
	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Handle error.\n";
	}


	m_activeActionSet.ulActionSet = m_mainSetHandler;
	m_activeActionSet.ulRestrictedToDevice = vr::k_ulInvalidInputValueHandle;
	// When I didn't manually set priority zero it didn't work for me, for unknown reasons.
	m_activeActionSet.nPriority = 0;

}

bool SteamIVRInput::nextSongSet()
{
	auto e = vr::VRInput()->GetDigitalActionData(m_nextSongHandler, &m_nextSongData, sizeof(m_nextSongData), vr::k_ulInvalidInputValueHandle);

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
	auto error = vr::VRInput()->UpdateActionState(
		&m_activeActionSet, sizeof(m_activeActionSet), 1);

	if (error != vr::EVRInputError::VRInputError_None)
	{
		std::cerr << "Loop error.\n";
	}


	vr::VRInput()->GetDigitalActionData(m_RightTriggerButtonHandler, &tri_right, sizeof(tri_right), vr::k_ulInvalidInputValueHandle);
	vr::VRInput()->GetDigitalActionData(m_LeftTriggerButtonHandler, &tri_left, sizeof(tri_left), vr::k_ulInvalidInputValueHandle);
	//LOG_F(WARNING, "trigger \t%d, \t%d", tri_left.bChanged, tri_right.bChanged);

	vr::VRInput()->GetDigitalActionData(m_RightGripButtonHandler, &grip_right, sizeof(grip_right), vr::k_ulInvalidInputValueHandle);
	vr::VRInput()->GetDigitalActionData(m_LeftGripButtonHandler, &grip_left, sizeof(grip_left), vr::k_ulInvalidInputValueHandle);
	////LOG_F(WARNING, "grip \t%d, \t%d", grip_left.bState, grip_right.bState);

	vr::VRInput()->GetDigitalActionData(m_RightMenuButtonHandler, &menu_right, sizeof(menu_right), vr::k_ulInvalidInputValueHandle);
	vr::VRInput()->GetDigitalActionData(m_LeftMenuButtonHandler, &menu_left, sizeof(menu_left), vr::k_ulInvalidInputValueHandle);
	////LOG_F(WARNING, "menu_left \t%d, \t%d", menu_left.bState, menu_right.bState);

	vr::VRInput()->GetAnalogActionData(m_RightAnalogHandler, &analog_right, sizeof(analog_right), vr::k_ulInvalidInputValueHandle);
	vr::VRInput()->GetAnalogActionData(m_LeftAnalogHandler, &analog_left, sizeof(analog_left), vr::k_ulInvalidInputValueHandle);
	//LOG_F(WARNING, "analog \t%f, \t%f", analog_right.x, analog_right.y);
}

std::map<vr::EVRButtonId, vr::InputAnalogActionData_t> SteamIVRInput::GetAllAnalogEvents(vr::ETrackedControllerRole ControllerRole)
{
	std::map<vr::EVRButtonId, vr::InputAnalogActionData_t> AnalogEvents{};
	switch (ControllerRole)
	{
	case vr::TrackedControllerRole_LeftHand:
		AnalogEvents.insert(std::pair<vr::EVRButtonId, vr::InputAnalogActionData_t>(vr::EVRButtonId::k_EButton_A, analog_left));
		break;

	case vr::TrackedControllerRole_RightHand:
		AnalogEvents.insert(std::pair< vr::EVRButtonId, vr::InputAnalogActionData_t>(vr::EVRButtonId::k_EButton_A, analog_right));
		break;

	default:
		break;
	}

	return AnalogEvents;
}

std::map<vr::EVRButtonId, vr::InputDigitalActionData_t> SteamIVRInput::GetAllDigitalEvents(vr::ETrackedControllerRole ControllerRole)
{
	std::map<vr::EVRButtonId, vr::InputDigitalActionData_t> DigitalEvents{};
	switch (ControllerRole)
	{
	case vr::TrackedControllerRole_LeftHand:
		DigitalEvents.insert(std::pair<vr::EVRButtonId, vr::InputDigitalActionData_t>(vr::EVRButtonId::k_EButton_ApplicationMenu, menu_left));
		DigitalEvents.insert(std::pair<vr::EVRButtonId, vr::InputDigitalActionData_t>(vr::EVRButtonId::k_EButton_Grip, grip_left));
		DigitalEvents.insert(std::pair<vr::EVRButtonId, vr::InputDigitalActionData_t>(vr::EVRButtonId::k_EButton_SteamVR_Trigger, tri_left));
		break;

	case vr::TrackedControllerRole_RightHand:
		DigitalEvents.insert(std::pair<vr::EVRButtonId, vr::InputDigitalActionData_t>(vr::EVRButtonId::k_EButton_ApplicationMenu, menu_right));
		DigitalEvents.insert(std::pair<vr::EVRButtonId, vr::InputDigitalActionData_t>(vr::EVRButtonId::k_EButton_Grip, grip_right));
		DigitalEvents.insert(std::pair<vr::EVRButtonId, vr::InputDigitalActionData_t>(vr::EVRButtonId::k_EButton_SteamVR_Trigger, tri_right));
		break;

	default:
		break;
	}

	return DigitalEvents;
}


bool SteamIVRInput::GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath)
{
	vr::InputDigitalActionData_t actionData;
	vr::VRInput()->GetDigitalActionData(m_nextSongHandler, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
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
