#pragma once

/// STL
#include <memory>
#include <string>
#include <map>

/// Dependency includes
#include <OpenVR/openvr.h>

/// Local includes
#include <VR/VRInitializable.h>

namespace VR {
	namespace Implementation
	{
		class VRCore : VRInitializable {
		public:
			bool InitializeCore();

			bool CoreInitialized();
			std::shared_ptr<vr::IVRSystem> GetVrSystem();

		protected:
			//

		private:
			bool IsCoreInitialized = false;

			bool InitializeCompositor();

			std::shared_ptr<vr::IVRSystem> VrSystem;
		};
	}
}
