#pragma once

/// STL
#include <map>
#include <string>
#include <vector>
#include <memory>

/// Dependency includes
#include <OpenVR/openvr.h>

/// Local includes
#include <VR/VRCore.h>
#include <VR/vrDataProviders/vrProvidedData/ProvidedData.h>
#include "Utils.h"

namespace VR
{
	namespace Implementation
	{
		class VRInput : VRInitializable
		{
		public:
			VRInput(const std::shared_ptr<VRCore> VrCore) : VrCore{ VrCore }
			{
				//
			}

			bool InitModule() override;

			bool DetectControllers();
			bool DetectEvents(vr::ETrackedControllerRole ControllerRole);

			std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> GetDetectedControllers();
			std::vector<vr::VREvent_t> GetDetectedEvents(vr::ETrackedControllerRole ControllerRole);

			bool ControllersAlreadyDetected();

		protected:
			//

		private:
			std::shared_ptr<VRCore> VrCore;
			vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
			std::pair<vr::TrackedDeviceIndex_t, vr::TrackedDeviceIndex_t> DetectedControllers{ vr::k_unTrackedDeviceIndexInvalid, vr::k_unTrackedDeviceIndexInvalid };
			std::vector<vr::VREvent_t> DetectedEvents;
		};
	}
}
