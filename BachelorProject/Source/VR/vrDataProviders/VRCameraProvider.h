#pragma once

#include <VR/VRInitializable.h>
#include <VR/VRGeometry.h>
#include <VR/VRCore.h>

#include "Provider.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		class VRCameraProvider final : VR::Implementation::VRInitializable, Provider<ProvidedDataType>
		{
		public:
			VRCameraProvider();

			bool InitModule() override;
			bool ReceiveBroadcastData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			atd::shared_ptr<VR::Implementation::VRCore> VrCore;
			std::shared_ptr<VR::VRGeometry> VrGeometry;
		};
	}
}
