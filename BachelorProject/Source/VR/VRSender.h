#pragma once

#include <memory>

#include <OpenVR/openvr.h>

#include <VR/VRInitializable.h>
#include <VR/VRCore.h>

namespace VR
{
	namespace Implementation
	{
		class VRSender : VRInitializable
		{
		public:
			VRSender();

			bool InitModule() override;

			bool SubmitTexturesToHMD(vr::Texture_t leftTexture, vr::Texture_t rightTexture);
			bool TriggerVibration(vr::TrackedDeviceIndex_t Controller, unsigned short MillisecondsDuration);
		protected:
			//

		private:
			std::shared_ptr<VR::Implementation::VRCore> VrCore;
		};
	}
}
