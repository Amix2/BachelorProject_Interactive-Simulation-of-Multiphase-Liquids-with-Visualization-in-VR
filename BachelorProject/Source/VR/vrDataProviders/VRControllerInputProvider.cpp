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

		bool VRControllerInputProvider::ReceiveData()
		{
			this->VrInput->DetectEvents();
			return true;
		}

		bool VRControllerInputProvider::IsReceivedDataStillValid() const
		{
			return true;
		}
	}
}
