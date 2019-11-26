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
	/**
		The core VR class, responsible for initializing the most important OpenVR modules.
	*/
	class VRCore : VRInitializable {
	public:
		/**
			Initializes every obligatory, as well as any used in the project, OpenVR module.
			
			@return Whether this operation has been completed successfully or not.
		*/
		bool InitializeCore();

		/**
			Returns whether the core has already been initialized or not.

			@return Whether the core was initialized or not.
		*/
		bool CoreInitialized();

		/**
			Returns the pointer to the IVRSystem from OpenVR.

			@return The IVRPointer from OpenVR.
		*/
		std::shared_ptr<vr::IVRSystem> GetVrSystem();

		//bool SubmitTexturesToHMD(vr::Texture_t leftTexture, vr::Texture_t rightTexture);

	protected:
		//

	private:
		/// Information about whether the core was already initialized or not.
		bool IsCoreInitialized = false;

		/**
			Initializes the OpenVR compositor.

			@return Whether this operation has been completed successfully or not.
		*/
		bool InitializeCompositor();

		// bool InitializeDescriptors();
		/// The IVR pointer from OpenVR.
		std::shared_ptr<vr::IVRSystem> VrSystem;
		// std::map<vr::Hmd_Eye, std::map<std::string, int>> Descriptors;
	};
}
