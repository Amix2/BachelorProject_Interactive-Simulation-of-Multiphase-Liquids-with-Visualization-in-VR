#pragma once

#include <OpenVR/openvr.h>

#include <VR/VRCore.h>

namespace VR
{
	namespace UploadedDataTypes
	{
		class HapticFeedbackData
		{
		public:
			vr::TrackedDeviceIndex_t ControllerIndex;
			unsigned short MicrosecondsDuration;

			void SetControllerIndex(vr::TrackedDeviceIndex_t ControllerIndex)
			{
				this->ControllerIndex = ControllerIndex;
			}

			void SetMicrosecondsDuration(unsigned short MicrosecondsDuration)
			{
				this->MicrosecondsDuration = MicrosecondsDuration;
			}

		protected:
			//

		private:
			//
		};
	}
}
