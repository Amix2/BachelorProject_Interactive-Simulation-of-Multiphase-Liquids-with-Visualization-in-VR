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

			bool RegisterListeners(std::map<VREventClassifiedType, VREventListener> Listeners);
			bool UnregisterListeners(std::vector<VREventClassifiedType> ListenerNames);

			bool ClearBuffers();

		protected:
			//

		private:
			std::map<VREventClassifiedType, VREventListener> VrEventListeners{};

			std::shared_ptr<VR::Implementation::VRCore> VrCore{};

			std::shared_ptr<VREventPoller> VrEventPoller{};
			std::vector<vr::VREvent_t> VrEvents{};
			std::map<VREventClassifiedType, std::vector<vr::VREvent_t> > VrClassifiedEvents{};

			bool ClassifyEvents();
		};
	}
}
