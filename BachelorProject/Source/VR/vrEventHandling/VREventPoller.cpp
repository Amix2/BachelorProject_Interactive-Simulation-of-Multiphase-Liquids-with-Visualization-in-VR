#include "VREventPoller.h"

namespace VR
{
	namespace EventHandling
	{
		VREventPoller::VREventPoller()
		{
			//
		}

		bool VREventPoller::InitModule()
		{
			return true;
		}

		std::vector<vr::VREvent_t> VREventPoller::PollAllEvents()
		{
			this->VrInput->DetectEvents();
			return this->VrInput->GetDetectedEvents();
		}

		std::vector<vr::VREvent_t> VREventPoller::RunParallel()
		{
			this->VrInput->DetectEvents();
			return this->VrInput->GetDetectedEvents();
		}
	}
}