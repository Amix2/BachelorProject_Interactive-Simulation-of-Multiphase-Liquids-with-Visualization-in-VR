#include "VRCore.h"
#include <iostream>

namespace VR {
	bool VRCore::InitializeCore() {
		vr::EVRInitError VrError = vr::VRInitError_None;
		this->VrSystem = vr::VR_Init(&VrError, vr::VRApplication_Scene);

		if (VrError != vr::VRInitError_None) {
			std::cout << vr::VR_GetVRInitErrorAsEnglishDescription(VrError) << std::endl;
			return false;
		}

		if (!this->InitializeCompositor()) {
			return false;
		}

		return true;
	}

	bool VRCore::InitializeCompositor() {
		if (!vr::VRCompositor()) {
			return false;
		}

		return true;
	}

	bool VRCore::InitializeDescriptors() {
		//
		this->Descriptors[vr::Eye_Left] = {
			{}
		};
		this->Descriptors[vr::Eye_Right] = {
			{}
		};

		return true;
	}

	vr::IVRSystem * VRCore::GetVrSystem() {
		return this->VrSystem;
	}

	bool VRCore::SubmitTexturesToHMD(vr::Texture_t LeftTexture, vr::Texture_t RightTexture) {
		vr::VRCompositor()->Submit(vr::Eye_Left, &LeftTexture);
		vr::VRCompositor()->Submit(vr::Eye_Right, &RightTexture);

		return true;
	}
}
