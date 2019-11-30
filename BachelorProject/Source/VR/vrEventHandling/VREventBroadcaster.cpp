#include <VR/vrEventHandling/VREventClassifier.h>

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
			this->VrEventPoller = std::make_shared<VREventPoller>(new VREventPoller());
			return true;
		}

		bool VREventBroadcaster::BroadcastEvents() throw(VREventException)
		{
			for (auto& VREventListener : this->VREventListeners)
			{
				VREventListener.second.ReceiveBroadcastData();
			}
			return true;
		}

		bool VREventBroadcaster::GatherEvents() throw(VREventException)
		{
			this->VrEvents = this->VrEventPoller->PollAllEvents();
			return true;
		}

		bool VREventBroadcaster::RegisterListeners(std::map<std::string, ProvidedDataTypes::ProvidedData> Listeners)
		{
			this->VREventListeners.insert(std::begin(Listeners), std::end(Listeners));
			return true;
		}

		bool VREventBroadcaster::UnregisterListeners(std::vector<std::string> ListenerNames)
		{
			for (auto& ListenerName : ListenerNames)
			{
				this->VREventListeners.erase(ListenerName);
			}
			return true;
		}

		bool VREventBroadcaster::ClassifyEvents()
		{
			return false;
		}
	}
}
