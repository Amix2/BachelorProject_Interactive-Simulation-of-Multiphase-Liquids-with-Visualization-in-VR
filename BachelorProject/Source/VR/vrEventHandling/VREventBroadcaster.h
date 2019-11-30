#pragma once

#include <string>
#include <vector>
#include <map>

#include <VR/VRInitializable.h>
#include <VR/vrDataProviders/vrProvidedData/ProvidedData.h>
#include <VR/vrEventHandling/VREventPoller.h>
#include <VR/vrEventHandling/VREventListener.h>
#include <VR/vrEventHandling/VREventParallelHandler.h>
#include <VR/vrEventHandling/VREventClassifier.h>

namespace VR
{
	namespace EventHandling
	{
		class VREventBroadcaster : VR::Implementation::VRInitializable, VREventParallelHandler<std::vector<vr::VREvent_t>>
		{
		public:
			VREventBroadcaster(std::shared_ptr<VR::Implementation::VRCore> VrCore);

			bool InitModule();

			bool BroadcastClassifiedEvents() throw(VREventException);
			bool GatherEvents() throw(VREventException);

			bool RegisterListeners(std::map<std::string, ProvidedDataTypes::ProvidedData> Listeners);
			bool UnregisterListeners(std::vector<std::string> ListenerNames);

		protected:
			//

		private:
			std::map<VREventClassifiedType, VREventListener<VR::ProvidedDataTypes::ProvidedData>> VrEventListeners{};

			std::shared_ptr<VR::Implementation::VRCore> VrCore{};

			std::shared_ptr<VREventPoller> VrEventPoller{};
			std::vector<vr::VREvent_t> VrEvents{};

			bool ClassifyEvents();
		};
	}
}
