#pragma once

#include <future>

#include <VR/VRInitializable.h>
#include <VR/vrEventHandling/VREventParallelHandler.h>

#include "VREventException.h"

namespace VR
{
	namespace EventHandling
	{
		template <typename BroadcastResult>
		class VREventListener : VRInitializable, VREventParallelHandler
		{
		public:
			VREventListener();

			virtual BroadcastResult ReceiveBroadcastData() throw(VREventException) = 0;

		protected:
			//

		private:
			BroadcastResult BlockAndListen() throw(VREventException);
			std::future<BroadcastResult> ListenWithoutBlocking() throw(VREventException);
		};
	}
}
