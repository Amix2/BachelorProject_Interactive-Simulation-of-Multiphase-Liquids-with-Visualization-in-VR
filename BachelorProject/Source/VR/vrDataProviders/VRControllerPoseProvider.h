#pragma once

#include <OpenVR/openvr.h>

#include <VR/VRInitializable.h>
#include <VR/vrEventHandling/VREventListener.h>

#include "Provider.h"
#include "vrProvidedData/ControllerPoseData.h"

namespace VR
{
	namespace DataProviders
	{
		class VRControllerPoseProvider final : VR::Implementation::VRInitializable, Provider<ProvidedDataTypes::ControllerPoseData>, VREventListener
		{
		public:
			vr::ETrackedControllerRole ControllerRole;
			VRControllerPoseProvider();
			bool init() override;
			bool ReceiveBroadcastData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			glm::mat4 ControllerPose;
		};
	}
}
