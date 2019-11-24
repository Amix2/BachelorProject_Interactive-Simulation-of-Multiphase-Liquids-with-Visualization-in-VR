#include "VRControllerProvider.h"

namespace VR
{
	namespace DataProviders
	{
		void VRControllerProvider::SetControllerRole(vr::ETrackedControllerRole ControllerRole)
		{
			this->ControllerRole = ControllerRole;
		}

		void VRControllerProvider::SetProviderMode(VRControllerProviderMode ProviderMode)
		{
			this->ProviderMode = ProviderMode;
		}

		VRControllerProvider::VRControllerProvider(VRControllerProviderMode ProviderMode)
		{
			this->ProviderMode = ProviderMode;
		}

		bool VRControllerProvider::init()
		{
			bool InitSuccessful = this->VRControllerInputProvider->ReceiveData() && this->VRControllerPoseProvider->ReceiveData();
			return InitSuccessful;
		}

		bool VRControllerProvider::ReceiveData()
		{
			bool DataReceived = this->VRControllerInputProvider->ReceiveData() && this->VRControllerPoseProvider->ReceiveData();
			return DataReceived;
		}

		bool VRControllerProvider::IsReceivedDataStillValid() const
		{
			bool ReceivedDataStillValid = this->VRControllerInputProvider->IsReceivedDataStillValid() && this->VRControllerPoseProvider->IsReceivedDataStillValid();
			return ReceivedDataStillValid;
		}
	}
}