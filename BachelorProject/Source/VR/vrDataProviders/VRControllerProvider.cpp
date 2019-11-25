#include "VRControllerProvider.h"

namespace VR
{
	namespace DataProviders
	{
		/*
		void VRControllerProvider::SetControllerRole(vr::ETrackedControllerRole ControllerRole)
		{
			this->ControllerRole = ControllerRole;
		}
		*/

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
			bool InitSuccessful;
			switch (this->ProviderMode)
			{
			case VRControllerProviderMode::ONLY_INPUT:
				InitSuccessful = this->VRControllerInputProvider->init();
				break;

			case VRControllerProviderMode::ONLY_POSE:
				InitSuccessful = this->VRControllerPoseProvider->init();
				break;

			case VRControllerProviderMode::BOTH:
			{
				InitSuccessful = this->VRControllerInputProvider->init();
				InitSuccessful |= this->VRControllerPoseProvider->init();
				break;
			}
				
			default:
				InitSuccessful = false;
				break;
			}
			return InitSuccessful;
		}

		bool VRControllerProvider::ReceiveData()
		{
			const bool IsDataReceived = this->VRControllerInputProvider->ReceiveData() && this->VRControllerPoseProvider->ReceiveData();
			const ProvidedDataTypes::ControllerData ControllerData{ this->VRControllerInputProvider->GetProvidedData(), this->VRControllerPoseProvider->GetProvidedData() };
			this->ProvidedData = ControllerData;
			return IsDataReceived;
		}

		bool VRControllerProvider::IsReceivedDataStillValid() const
		{
			bool ReceivedDataStillValid = this->VRControllerInputProvider->IsReceivedDataStillValid() && this->VRControllerPoseProvider->IsReceivedDataStillValid();
			return ReceivedDataStillValid;
		}
	}
}