#include "VRCore.h"
#include <iostream>

namespace VR {
	namespace Implementation
	{
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

			this->IsCoreInitialized = true;

			return true;
		}

		bool VRCore::CoreInitialized()
		{
			return this->IsCoreInitialized;
		}

		bool VRCore::InitializeCompositor() {
			if (!vr::VRCompositor()) {
				return false;
			}

			return true;
		}

		std::shared_ptr<vr::IVRSystem> VRCore::GetVrSystem() {
			return this->VrSystem;
		}
	}
}
