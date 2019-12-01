#include <VR/vrEventHandling/VREventClassifier.h>
#include <VR/vrEventHandling/VREventClassifiedType.h>

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

		bool VREventBroadcaster::BroadcastClassifiedEvents() throw(VREventException)
		{
			for (auto& VrEventsByType : this->VrClassifiedEvents)
			{
				for (auto& VrClassifiedEvent : VrEventsByType.second)
				{
					VR::ProvidedDataTypes::ProvidedData* ProvidedData =
						VR::EventHandling::VREventClassifier::GenerateData(VrClassifiedEvent, VrEventsByType.first);
					if (ProvidedData == nullptr)
					{
						return false;
					}
					this->VrEventListeners[VrEventsByType.first].ReceiveBroadcastData(ProvidedData);
				}
			}

			return true;
		}

		bool VREventBroadcaster::GatherEvents() throw(VREventException)
		{
			this->VrEvents = this->VrEventPoller->PollAllEvents();
			return true;
		}

		bool VREventBroadcaster::RegisterListeners(std::map<VREventClassifiedType,VREventListener> Listeners)
		{
			this->VrEventListeners.insert(std::begin(Listeners), std::end(Listeners));
			return true;
		}

		bool VREventBroadcaster::UnregisterListeners(std::vector<VREventClassifiedType> ListenerNames)
		{
			for (auto& ListenerName : ListenerNames)
			{
				this->VrEventListeners.erase(ListenerName);
			}
			return true;
		}

		bool VREventBroadcaster::ClearBuffers()
		{
			this->VrEvents.clear();
			this->VrClassifiedEvents.clear();

			return true;
		}

		bool VREventBroadcaster::ClassifyEvents()
		{
			std::map<VREventClassifiedType, std::vector<vr::VREvent_t>> VrClassifiedEvents{};
			for (auto& VrEvent : this->VrEvents)
			{
				VREventClassifiedType Type = VR::EventHandling::VREventClassifier::ClassifyEvent(VrEvent);
				if (Type != VREventClassifiedType::VREVENT_INVALID)
				{
					if (VrClassifiedEvents.find(Type) == VrClassifiedEvents.end())
					{
						VrClassifiedEvents[Type] = std::vector<vr::VREvent_t>();
					}
					VrClassifiedEvents[Type].push_back(VrEvent);
				}
			}
			this->VrClassifiedEvents = VrClassifiedEvents;

			return true;
		}
	}
}
