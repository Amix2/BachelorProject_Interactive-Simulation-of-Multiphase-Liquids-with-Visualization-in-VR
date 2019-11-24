#pragma once

#include <VR/VRGeometry.h>

#include "Provider.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		class VRCameraProvider final : Provider<ProvidedDataType>
		{
		public:
			VRCameraProvider();
			bool init() override;
			bool ReceiveData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			std::shared_ptr<VR::VRGeometry> VrGeometry;
		};
	}
}
