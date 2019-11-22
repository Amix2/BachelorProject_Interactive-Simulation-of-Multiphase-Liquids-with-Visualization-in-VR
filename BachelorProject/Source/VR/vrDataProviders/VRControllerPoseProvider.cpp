#include "VRControllerPoseProvider.h"

namespace VR
{
	namespace DataProviders
	{
		template<typename ProvidedDataType>
		VRControllerPoseProvider<ProvidedDataType>::VRControllerPoseProvider()
		{
			//
		}

		template <typename ProvidedDataType>
		bool VRControllerPoseProvider<ProvidedDataType>::init()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VRControllerPoseProvider<ProvidedDataType>::ReceiveData()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VRControllerPoseProvider<ProvidedDataType>::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
