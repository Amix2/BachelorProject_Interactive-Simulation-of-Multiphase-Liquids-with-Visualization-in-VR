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
		class VRControllerInputProvider final : VR::Implementation::VRInitializable,
			Provider<ProvidedDataTypes::ControllerInputData>,
			VR::EventHandling::VREventListener
		{
		public:
			VRControllerInputProvider();
			bool InitModule() override;
			bool ReceiveBroadcastData(VR::ProvidedDataTypes::ProvidedData *ProvidedData) override;

		protected:
			//

		private:
			// std::shared_ptr<VR::Implementation::VRInput> VrInput;
		};
	}
}
