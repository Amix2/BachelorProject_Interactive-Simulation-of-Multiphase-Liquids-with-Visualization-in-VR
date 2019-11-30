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
		class VREventListener abstract : VREventParallelHandler
		{
		public:
			VREventListener();

			virtual bool ReceiveBroadcastData() throw(VREventException) = 0;

		protected:
			//

		private:
			//broadcastresult blockandlisten() throw(vreventexception);
			//std::future<broadcastresult> listenwithoutblocking() throw(vreventexception);
		};
	}
}
