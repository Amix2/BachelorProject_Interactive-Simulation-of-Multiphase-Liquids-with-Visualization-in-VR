#pragma once

#include <OpenVR/openvr.h>

#include <VR/VRCore.h>
#include <VR/vrUploaders/vrUploadedData/UploadData.h>

namespace VR
{
	namespace UploadDataTypes
	{
		class HapticFeedbackData : UploadData
		{
		public:
			vr::TrackedDeviceIndex_t ControllerIndex;
			unsigned short MicrosecondsDuration;

		protected:
			//

		private:
			//
		};
	}
}
