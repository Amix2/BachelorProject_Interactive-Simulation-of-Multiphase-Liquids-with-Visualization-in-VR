#pragma once

#include <memory>

#include <VR/VRInput.h>
#include <VR/VRInitializable.h>
#include <VR/vrEventHandling/VREventListener.h>

#include "Provider.h"
#include "vrProvidedData/ControllerInputData.h"

namespace VR
{
	namespace DataProviders
	{
		class VRControllerInputProvider final : VR::Implementation::VRInitializable, Provider<ProvidedDataTypes::ControllerInputData>
		{
		public:
			VRControllerInputProvider();
			bool InitModule() override;
			bool ReceiveBroadcastData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			std::shared_ptr<VRInput> VrInput;
		};
	}
}
