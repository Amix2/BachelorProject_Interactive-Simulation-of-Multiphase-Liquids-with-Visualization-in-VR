#pragma once

#include <future>

#include <VR/VRInitializable.h>
#include <VR/vrEventHandling/VREventParallelHandler.h>
#include <VR/vrEventHandling/VREventException.h>
#include <VR/vrDataProviders/Provider.h>

namespace VR
{
	namespace EventHandling
	{
		class VREventListener abstract
		{
		public:
			VREventListener()
			{
				//
			}

			virtual bool ReceiveBroadcastData(VR::ProvidedDataTypes::ProvidedData *ProvidedData) throw(VREventException) = 0;

		protected:
			//

		private:
			//broadcastresult blockandlisten() throw(vreventexception);
			//std::future<broadcastresult> listenwithoutblocking() throw(vreventexception);
		};
	}
}
