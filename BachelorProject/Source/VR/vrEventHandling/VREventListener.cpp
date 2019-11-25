#include "VREventListener.h"

#include <functional>
#include <thread>

namespace VR
{
	namespace EventHandling
	{
		template <typename BroadcastResult>
		BroadcastResult VREventListener<BroadcastResult>::Listen(ListenType ListenType) throw(VREventException)
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

		template <typename BroadcastResult>
		BroadcastResult VREventListener<BroadcastResult>::BlockAndListen() throw(VREventException)
		{
			//
		}

		template <typename BroadcastResult>
		std::future<BroadcastResult> VREventListener<BroadcastResult>::ListenWithoutBlocking() throw(VREventException)
		{
			std::future<BroadcastResult> FutureBroadcastResult{};
			std::function<BroadcastResult> AsynchronousFunction = [PromiseToReturnBroadcastResult]()
			{
				// TODO: fill out
			};
			AsyncBroadcastResult = std::async{ std::launch::async, AsynchronousFunction };

			return FutureBroadcastResult;
		}
	}
}
