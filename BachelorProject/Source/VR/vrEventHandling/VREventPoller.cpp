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
			return false;
		}

		std::vector<vr::VREvent_t> VREventPoller::CodeToRunInParallel()
		{
			return std::vector<vr::VREvent_t>();
		}
	}
}