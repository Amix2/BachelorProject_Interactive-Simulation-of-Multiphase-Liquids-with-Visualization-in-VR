#include "VRPoseProvider.h"

namespace VR
{
	namespace DataProviders
	{
		VR::DataProviders::VRPoseProvider::VRPoseProvider()
		{
			//
		}

		bool VR::DataProviders::VRPoseProvider::init()
		{
			return true;
		}

		bool VR::DataProviders::VRPoseProvider::ReceiveData()
		{
			return true;
		}

		bool VR::DataProviders::VRPoseProvider::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
