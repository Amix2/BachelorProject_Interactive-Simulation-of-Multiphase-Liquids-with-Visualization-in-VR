#include "VRSender.h"

namespace VR
{
	namespace Implementation
	{
		VRSender::VRSender()
		{
			//
		}

		bool VRSender::InitModule()
		{
			return true;
		}

		bool VRSender::TriggerVibration(vr::TrackedDeviceIndex_t Controller, unsigned short MillisecondsDuration)
		{
			this->VrCore->GetVrSystem->TriggerHapticPulse(Controller, 0, MillisecondsDuration);
		}

		bool VRSender::SubmitTexturesToHMD(vr::Texture_t LeftTexture, vr::Texture_t RightTexture) {
			vr::VRCompositor()->Submit(vr::Eye_Left, &LeftTexture);
			vr::VRCompositor()->Submit(vr::Eye_Right, &RightTexture);

			return true;
		}
	}
}
