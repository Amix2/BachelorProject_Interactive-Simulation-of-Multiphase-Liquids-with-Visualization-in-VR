#pragma once

#include <memory>

#include <VR/VRGLInterop.h>

#include "Provider.h"
#include "VRControllerInputProvider.h"
#include "VRControllerPoseProvider.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		class VRControllerProvider final : Provider<ProvidedDataType>
		{
		public:
			VRControllerProvider();
			bool init() override;
			bool ReceiveData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			vr::ETrackedControllerRole GetControllerRole() const;
			VR::VRGLInterop VRGLInterop{};

			std::shared_ptr<VRControllerPoseProvider> VRControllerPoseProvider;
			std::shared_ptr<VRControllerInputProvider> VRControllerInputProvider;
		};
	}
}
