#include "VRCore.h"
#include <iostream>

namespace VR {
	bool VRCore::InitializeCore() {
		vr::EVRInitError VrError = vr::VRInitError_None;
		this->VrSystem = vr::VR_Init(&VrError, vr::VRApplication_Scene);

		std::cout << "xxx" << std::endl;

		if (VrError != vr::VRInitError_None) {
			std::cout << vr::VR_GetVRInitErrorAsEnglishDescription(VrError) << std::endl;
			return false;
		}

		std::cout << "yyy" << std::endl;

		if (!this->InitializeCompositor()) {
			return false;
		}

		std::cout << "zzz" << std::endl;

		return true;
	}

	bool VRCore::InitializeCompositor() {
		if (!vr::VRCompositor()) {
			return false;
		}

		return true;
	}

	vr::IVRSystem * VRCore::GetVrSystem() {
		return this->VrSystem;
	}
}