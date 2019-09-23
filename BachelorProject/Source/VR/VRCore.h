#pragma once

#include <memory>

#include <OpenVR/openvr.h>

namespace VR {
	class VRCore {
	public:
		bool InitializeCore();
		vr::IVRSystem * GetVrSystem();

	protected:
		//

	private:
		bool InitializeCompositor();
		vr::IVRSystem *VrSystem;
	};
}
