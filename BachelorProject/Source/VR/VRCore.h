#pragma once

#include <memory>

#include <OpenVR/openvr.h>

namespace VR {
	class VRCore {
	public:
		bool InitializeCore();

	protected:
		//

	private:
		bool InitializeCompositor();
		// std::shared_ptr<vr::IVRSystem> VrSystem;
	};
}
