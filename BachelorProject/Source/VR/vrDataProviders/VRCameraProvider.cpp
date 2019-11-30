#include "VRCameraProvider.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		VRCameraProvider<ProvidedDataType>::VRCameraProvider()
		{
			//
		}

		template<typename ProvidedDataType>
		bool VRCameraProvider<ProvidedDataType>::InitModule()
		{
			return false;
		}

		template<typename ProvidedDataType>
		bool VRCameraProvider<ProvidedDataType>::ReceiveBroadcastData()
		{
			return false;
		}

		template <typename ProvidedDataType>
		bool VRCameraProvider<ProvidedDataType>::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
