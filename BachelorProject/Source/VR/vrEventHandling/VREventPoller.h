#pragma once

#include <memory>
#include <vector>

#include <OpenVR/openvr.h>

#include <VR/VRInitializable.h>
#include <VR/VRInput.h>
#include <VR/vrEventHandling/VREventParallelHandler.h>

namespace VR
{
	namespace EventHandling
	{
		class VREventPoller : VR::Implementation::VRInitializable
		{
		public:
			VREventPoller();

			bool InitModule();

			std::vector<vr::VREvent_t> PollAllEvents();

		protected:
			//

		private:
			std::shared_ptr<VR::Implementation::VRInput> VrInput;
		};
	}
}