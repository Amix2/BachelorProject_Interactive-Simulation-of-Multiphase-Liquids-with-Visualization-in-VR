#pragma once

#include "Provider.h"
#include "vrProvidedData/HMDPoseData.h"

namespace VR
{
	namespace DataProviders
	{
		class VRPoseProvider final : Provider<ProvidedDataTypes::HMDPoseData>
		{
		public:
			VRPoseProvider();
			bool init() override;
			bool ReceiveData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			//
		};
	}
}