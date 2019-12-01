#pragma once

#include <vector>
#include <algorithm>

#include <OpenVR/openvr.h>

#include <VR/vrDataProviders/vrProvidedData/ProvidedData.h>

namespace VR
{
	namespace ProvidedDataTypes
	{
		class ControllerInputData : public ProvidedData
		{
		public:
			ControllerInputData(vr::ETrackedControllerRole ControllerRole, std::vector<std::pair<vr::EVREventType, vr::EVRButtonId>> InputEvents)
				: ControllerRole{ ControllerRole }, InputEvents{ InputEvents }
			vr::ETrackedControllerRole ControllerRole;
			std::vector<std::pair<vr::EVREventType, vr::EVRButtonId>> InputEvents;

		protected:
			//

		private:
			//
		};
	}
}
