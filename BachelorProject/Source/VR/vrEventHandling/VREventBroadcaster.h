#pragma once

#include <string>

#include <VR/VRCore.h>
#include <VR/VRInitializable.h>

#include <VR/vrDataProviders/vrProvidedData/ProvidedData.h>

#include "VREventListener.h"

namespace VR
{
	namespace EventHandling
	{
		class VREventBroadcaster : VRInitializable
		{
		public:
			VREventBroadcaster(std::shared_ptr<VRCore> VrCore) : VrCore{ VrCore }
			{
				//
			}

			bool BroadcastEvents() throw(VREventException);
			bool GatherEvents() throw(VREventException);

			bool RegisterListeners(std::vector<std::pair<std::string, ProvidedDataTypes::ProvidedData>> Listeners);

		protected:
			//

		private:
			template <typename BroadcastEvent>
			std::vector<std::pair<std::string, ProvidedDataTypes::ProvidedData>> VREventListeners{};
			std::shared_ptr<VRCore> VrCore;
		};
	}
}
