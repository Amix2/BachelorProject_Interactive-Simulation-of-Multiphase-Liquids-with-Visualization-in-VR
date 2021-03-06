#pragma once

#include <memory>

#include <OpenVR/openvr.h>

#include <string>
#include <map>

namespace VR {
	class VRCore {
	public:
		bool InitializeCore();
		vr::IVRSystem * GetVrSystem();

		bool SubmitTexturesToHMD(vr::Texture_t leftTexture, vr::Texture_t rightTexture);

	protected:
		//

	private:
		bool InitializeCompositor();
		bool InitializeDescriptors();
		vr::IVRSystem *VrSystem;
		std::map<vr::Hmd_Eye, std::map<std::string, int>> Descriptors;
	};
}
