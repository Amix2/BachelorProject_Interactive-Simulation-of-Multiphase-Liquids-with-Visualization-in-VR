#include "VRControllerInputProvider.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		bool VRControllerInputProvider<ProvidedDataType>::init()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VRControllerInputProvider<ProvidedDataType>::ReceiveData()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VRControllerInputProvider<ProvidedDataType>::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
