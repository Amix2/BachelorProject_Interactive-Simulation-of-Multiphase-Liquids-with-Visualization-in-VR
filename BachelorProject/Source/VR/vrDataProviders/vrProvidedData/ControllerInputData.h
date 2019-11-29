#pragma once

#include <vector>
#include <algorithm>

#include <OpenVR/openvr.h>

namespace VR
{
	namespace ProvidedDataTypes
	{
		class ControllerInputData
		{
		public:
			ControllerInputData(vr::ETrackedControllerRole ControllerRole, )
			vr::ETrackedControllerRole ControllerRole;
			std::vector<std::pair<vr::EVREventType, vr::EVRButtonId>> ButtonEvents;

		protected:
			//

		private:
			//
		};
	}
}
