#include <VR/vrEventHandling/VREventListener.h>

#include "VRControllerInputProvider.h"

namespace VR
{
	namespace DataProviders
	{
		VRControllerInputProvider::VRControllerInputProvider()
		{
			//
		}

		bool VRControllerInputProvider::InitModule()
		{
			return true;
		}

		bool VRControllerInputProvider::ReceiveBroadcastData()
		{
			return false;
		}

		bool VRControllerInputProvider::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
