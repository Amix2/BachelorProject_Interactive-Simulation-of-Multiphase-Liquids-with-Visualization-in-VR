#include "VREventBroadcaster.h"

namespace VR
{
	namespace EventHandling
	{

		VREventBroadcaster::VREventBroadcaster(std::shared_ptr<VR::Implementation::VRCore> VrCore) : VrCore{ VrCore }
		{
			//
		}

		bool VREventBroadcaster::InitModule()
		{
			return false;
		}

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

		bool VREventBroadcaster::RegisterListeners(std::vector<std::pair<std::string, ProvidedDataTypes::ProvidedData>> Listeners)
		{
			return true;
		}

		bool VREventBroadcaster::UnregisterListeners(std::vector<std::string> ListenerNames)
		{
			return true;
		}
	}
}
