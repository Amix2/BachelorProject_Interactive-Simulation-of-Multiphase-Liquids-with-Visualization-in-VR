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

		template <typename ProvidedDataType>
		bool VRCameraProvider<ProvidedDataType>::init()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VRCameraProvider<ProvidedDataType>::ReceiveData()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VRCameraProvider<ProvidedDataType>::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
