#include "VRPoseProvider.h"

namespace VR
{
	namespace DataProviders
	{
		template<typename ProvidedDataType>
		VR::DataProviders::VRPoseProvider<ProvidedDataType>::VRPoseProvider()
		{
			this->init();
		}

		template <typename ProvidedDataType>
		bool VR::DataProviders::VRPoseProvider<ProvidedDataType>::init()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VR::DataProviders::VRPoseProvider<ProvidedDataType>::ReceiveData()
		{
			return true;
		}

		template <typename ProvidedDataType>
		bool VR::DataProviders::VRPoseProvider<ProvidedDataType>::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
