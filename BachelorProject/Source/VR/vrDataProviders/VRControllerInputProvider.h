#pragma once

#include <memory>
#include <VR/VRInput.h>
#include "Provider.h"
#include "vrProvidedData/ControllerInputData.h"

namespace VR
{
	namespace DataProviders
	{
		class VRControllerInputProvider final : Provider<ProvidedDataTypes::ControllerInputData>
		{
		public:
			VRControllerInputProvider();
			bool init() override;
			bool ReceiveData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			std::shared_ptr<VRInput> VrInput;
		};
	}
}
