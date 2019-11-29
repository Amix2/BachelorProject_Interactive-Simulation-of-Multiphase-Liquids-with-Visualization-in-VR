#pragma once

#include <string>
#include <vector>

#include <VR/VRInitializable.h>
#include <VR/vrDataProviders/vrProvidedData/ProvidedData.h>
#include <VR/vrEventHandling/VREventPoller.h>
#include <VR/vrEventHandling/VREventBuffer.h>
#include <VR/vrEventHandling/VREventListener.h>
#include <VR/vrEventHandling/VREventParallelHandler.h>

namespace VR
{
	namespace EventHandling
	{
		class VREventBroadcaster : VR::Implementation::VRInitializable, VREventParallelHandler
		{
		public:
			VREventBroadcaster(std::shared_ptr<VREventPoller> VrEventPoller);

			bool InitModule();

			bool BroadcastEvents() throw(VREventException);
			bool GatherEvents() throw(VREventException);

			bool RegisterListeners(std::vector<std::pair<std::string, ProvidedDataTypes::ProvidedData>> Listeners);
			bool UnregisterListeners(std::vector<std::string> ListenerNames);

		protected:
			//

		private:
			std::vector<std::pair<std::string, ProvidedDataTypes::ProvidedData>> VREventListeners{};

			std::shared_ptr<VREventPoller> VrEventPoller;
			std::shared_ptr<VREventBuffer> VrEventBuffer;
		};
	}
}
