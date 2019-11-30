#pragma once

#include <memory>

#include <VR/VRInitializable.h>
#include <VR/VRGLInterop.h>
#include <VR/vrEventHandling/VREventListener.h>

#include "Provider.h"
#include "VRControllerInputProvider.h"
#include "VRControllerPoseProvider.h"
#include "vrProvidedData/ControllerData.h"

namespace VR
{
	namespace DataProviders
	{
		enum class VRControllerProviderMode
		{
			ONLY_POSE,
			ONLY_INPUT,
			BOTH
		};

		class VRControllerProvider final : VR::Implementation::VRInitializable, Provider<ProvidedDataTypes::ControllerData>, VR::EventHandling::VREventListener
		{
		public:
			VRControllerProvider(VRControllerProviderMode ProviderMode);

			bool InitModule() override;
			bool ReceiveBroadcastData() override;
			bool IsReceivedDataStillValid() const override;
			//void SetControllerRole(vr::ETrackedControllerRole ControllerRole);
			void SetProviderMode(VRControllerProviderMode ProviderMode);

		protected:
			//

		private:
			VR::VRGLInterop VRGLInterop{};
			//vr::ETrackedControllerRole ControllerRole;
			VRControllerProviderMode ProviderMode;

			std::shared_ptr<VRControllerPoseProvider> VRControllerPoseProvider;
			std::shared_ptr<VRControllerInputProvider> VRControllerInputProvider;
		};
	}
}
