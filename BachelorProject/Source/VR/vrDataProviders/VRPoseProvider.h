#pragma once

#include <VR/VRInitializable.h>
#include <VR/vrEventHandling/VREventListener.h>

#include "Provider.h"
#include "vrProvidedData/HMDPoseData.h"

namespace VR
{
	namespace DataProviders
	{
		class VRPoseProvider final : VR::Implementation::VRInitializable, Provider<ProvidedDataTypes::HMDPoseData>, VR::EventHandling::VREventListener< ProvidedDataTypes::HMDPoseData>
		{
		public:
			VRPoseProvider();

			bool InitModule() override;
			bool ReceiveBroadcastData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			//
		};
	}
}