#include "VREventBroadcaster.h"

namespace VR
{
	namespace EventHandling
	{
		bool VREventBroadcaster::BroadcastEvents() throw(VREventException)
		{
			for (auto& VREventListener : this->VREventListeners)
			{
				//
			}
			return true;
		}

		bool VREventBroadcaster::GatherEvents() throw(VREventException)
		{
			return true;
		}

		bool VREventBroadcaster::RegisterListeners(std::vector<std::pair<std::string, >> Listeners)
		{
			return true;
		}
	}
}
