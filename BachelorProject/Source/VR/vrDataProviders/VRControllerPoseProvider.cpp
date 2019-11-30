#include "VRControllerPoseProvider.h"

namespace VR
{
	namespace DataProviders
	{
		VRControllerPoseProvider::VRControllerPoseProvider()
		{
			//
		}

		bool VRControllerPoseProvider::InitModule()
		{
			return true;
		}

		bool VRControllerPoseProvider::ReceiveData()
		{

			return true;
		}

		bool VRControllerPoseProvider::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
