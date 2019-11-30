#pragma once

#include <string>
#include <vector>
#include <map>

#include <VR/VRInitializable.h>
#include <VR/vrDataProviders/vrProvidedData/ProvidedData.h>
#include <VR/vrEventHandling/VREventPoller.h>
#include <VR/vrEventHandling/VREventListener.h>
#include <VR/vrEventHandling/VREventParallelHandler.h>

namespace VR
{
	namespace EventHandling
	{
		class VREventBroadcaster : VR::Implementation::VRInitializable, VREventParallelHandler<std::vector<vr::VREvent_t>>
		{
		public:
			VREventBroadcaster(std::shared_ptr<VR::Implementation::VRCore> VrCore);

			bool InitModule();

			bool BroadcastEvents() throw(VREventException);
			bool GatherEvents() throw(VREventException);

			bool RegisterListeners(std::map<std::string, ProvidedDataTypes::ProvidedData> Listeners);
			bool UnregisterListeners(std::vector<std::string> ListenerNames);

		protected:
			//

		private:
			std::map<std::string, VREventListener<VR::ProvidedDataTypes::ProvidedData>> VREventListeners{};

			std::shared_ptr<VR::Implementation::VRCore> VrCore;

			std::shared_ptr<VREventPoller> VrEventPoller{};
			std::shared_ptr<vr::VREvent_t> VrEvents{};

			bool ClassifyEvents();
		};
	}
}
