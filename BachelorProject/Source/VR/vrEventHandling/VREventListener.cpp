#include "VREventListener.h"

#include <functional>
#include <thread>

namespace VR
{
	namespace EventHandling
	{
		template <typename BroadcastResult>
		VREventListener<BroadcastResult>::VREventListener()
		{
			//
		}

		template <typename BroadcastResult>
		BroadcastResult VREventListener<BroadcastResult>::ReceiveBroadcastData() throw(VREventException)
		{
			BroadcastResult BroadcastResult{};
			switch (blocking)
			{
			case BLOCKING:
				BroadcastResult = this->BlockAndListen();
				break;

			case NON_BLOCKING:
				BroadcastResult = this->ListenWithoutBlocking();
				break;

			default:
				throw VREventException("Improper listening type. BLOCKING and NON_BLOCKING allowed only. Check out the ListenType enum class.\n");
			}

			return BroadcastResult;
		}
	}
}
