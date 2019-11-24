#pragma once

#include <OpenVR/openvr.h>

#include "Provider.h"
#include "vrProvidedData/ControllerPoseData.h"

namespace VR
{
	namespace DataProviders
	{
		class VRControllerPoseProvider final : Provider<ProvidedDataTypes::ControllerPoseData>
		{
		public:
			vr::ETrackedControllerRole ControllerRole;
			VRControllerPoseProvider();
			bool init() override;
			bool ReceiveData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			glm::mat4 ControllerPose;
		};
	}
}
