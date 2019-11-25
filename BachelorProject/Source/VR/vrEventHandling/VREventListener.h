#pragma once

#include <future>

#include <VR/VRInitializable.h>

#include "VREventException.h"

namespace VR
{
	namespace EventHandling
	{
		enum class ListenType
		{
			BLOCKING,
			NON_BLOCKING
		};

		template <typename BroadcastResult>
		class VREventListener : VRInitializable
		{
		public:
			VREventListener()
			{
				//
			}

			BroadcastResult Listen(ListenType blocking) throw(VREventException);

		protected:
			//

		private:
			BroadcastResult BlockAndListen() throw(VREventException);
			std::future<BroadcastResult> ListenWithoutBlocking() throw(VREventException);
		};
	}
}
