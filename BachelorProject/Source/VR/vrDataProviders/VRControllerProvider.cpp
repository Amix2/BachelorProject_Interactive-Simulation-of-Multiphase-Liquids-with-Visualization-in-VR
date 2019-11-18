#include "VRControllerProvider.h"


namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		vr::ETrackedControllerRole VRControllerProvider<ProvidedDataType>::GetControllerRole() const
		{
			return
		}

		template<typename ProvidedDataType>
		VRControllerProvider<ProvidedDataType>::VRControllerProvider()
		{
		}

		template <typename ProvidedDataType>
		bool VRControllerProvider<ProvidedDataType>::init()
		{
			bool InitSuccessful = this->VRControllerInputProvider->ReceiveData() && this->VRControllerPoseProvider->ReceiveData();
			return InitSuccessful;
		}

		template <typename ProvidedDataType>
		bool VRControllerProvider<ProvidedDataType>::ReceiveData()
		{
			bool DataReceived = this->VRControllerInputProvider->ReceiveData() && this->VRControllerPoseProvider->ReceiveData();
			return DataReceived;
		}

		template <typename ProvidedDataType>
		bool VRControllerProvider<ProvidedDataType>::IsReceivedDataStillValid() const
		{
			bool ReceivedDataStillValid = this->VRControllerInputProvider->IsReceivedDataStillValid() && this->VRControllerPoseProvider->IsReceivedDataStillValid();
			return ReceivedDataStillValid;
		}
	}
}