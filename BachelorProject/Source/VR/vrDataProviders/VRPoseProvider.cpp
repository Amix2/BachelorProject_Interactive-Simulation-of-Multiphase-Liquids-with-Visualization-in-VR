#include "VRPoseProvider.h"

namespace VR
{
	namespace DataProviders
	{
		VR::DataProviders::VRPoseProvider::VRPoseProvider()
		{
			//
		}

		bool VR::DataProviders::VRPoseProvider::InitModule()
		{
			return true;
		}

		bool VR::DataProviders::VRPoseProvider::ReceiveBroadcastData()
		{
			return true;
		}

		bool VR::DataProviders::VRPoseProvider::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
