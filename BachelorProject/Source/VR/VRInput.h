#pragma once

#include <map>
#include <string>
#include <vector>

#include <OpenVR/openvr.h>

#include <memory>

#include <VR/vrDataProviders/vrProvidedData/ProvidedData.h>

#include "VRCore.h"

#include "Utils.h"

namespace VR {
	class VRInput
	{
	public:
		enum class ControllerInitialization
		{
			NOT_INITIALIZED = -1,
			INITIALIZED = vr::k_unMaxTrackedDeviceCount
		};

		VRInput(const std::shared_ptr<VRCore> VRCore) : VRCore{ VRCore }
		{
			//
		}
		bool init();
		bool DetectControllers();
		bool DetectEvents(vr::ETrackedControllerRole ControllerRole);
		std::pair<vr::ETrackedControllerRole, vr::ETrackedControllerRole> GetDetectedControllers();
		std::vector<std::pair<vr::EVREventType, vr::EVRButtonId>> GetDetectedEvents(vr::ETrackedControllerRole ControllerRole);

	protected:
		//

	private:
		std::shared_ptr<VRCore> VRCore;
		vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
		std::pair<vr::ETrackedControllerRole, vr::ETrackedControllerRole> DetectedControllers{ -1, -1 };
		std::pair<vr::ETrackedControllerRole, std::vector<std::pair<vr::EVREventType, vr::EVRButtonId>>> DetectedEvents;
	};
}
