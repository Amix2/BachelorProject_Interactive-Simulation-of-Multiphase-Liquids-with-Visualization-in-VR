#pragma once

#include <vector>

#include <OpenVR/openvr.h>

#include <VR/VRInitializable.h>

namespace VR
{
	namespace EventHandling
	{
		class VREventBuffer : Implementation::VRInitializable
		{
		public:
			bool InitModule();

		protected:
			//

		private:
			std::vector<vr::VREvent_t> VrEvents;
		};
	}
}