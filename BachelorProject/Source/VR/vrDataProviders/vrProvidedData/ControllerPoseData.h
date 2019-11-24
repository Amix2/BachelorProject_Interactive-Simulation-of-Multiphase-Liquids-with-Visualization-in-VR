#pragma once

#include <glm/glm.hpp>
#include <OpenVR/openvr.h>

#include "ProvidedData.h"

namespace VR
{
	namespace ProvidedDataTypes
	{
		class ControllerPoseData : ProvidedData
		{
		public:
			glm::mat4 ControllerPose;
			vr::ETrackedDeviceClass ControllerHand;

		protected:
			//

		private:
			//
		};
	}
}
