#include "VRCore.h"

namespace VR {
	bool VRCore::InitializeCore() {
		vr::EVRInitError VrError = vr::VRInitError_None;
		vr::VR_Init(&VrError, vr::VRApplication_Scene);

		if (VrError != vr::VRInitError_None) {
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
}