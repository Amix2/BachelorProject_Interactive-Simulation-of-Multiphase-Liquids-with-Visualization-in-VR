#include "VRPoseProvider.h"

template<typename ProvidedDataType>
VR::DataProviders::VRPoseProvider<ProvidedDataType>::VRPoseProvider()
{
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
