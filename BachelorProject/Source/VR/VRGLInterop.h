#pragma once

#include <GL/glew.h>
#include <memory>
#include <VR/VRGeometry.h>
#include <VR/VRInput.h>
#include <VR/VRCore.h>
#include <scene/camera/VRCameraController.h>

namespace VR {
	class VRGLInterop {
	public:
		VRGLInterop(const std::shared_ptr<VRCore> VRCore) : VRCore{ VRCore };
		VRGLInterop(const VRGLInterop& vrglinterop) = default;
		VRGLInterop() = default;
		VRGLInterop& operator=(const VRGLInterop&) = default;
		std::shared_ptr<VR::VRCore> VrCore = std::make_shared<VR::VRCore>();
		std::shared_ptr<VR::VRGeometry> VrGeometry = std::make_shared<VR::VRGeometry>();
		std::shared_ptr<VR::VRInput> VrInput = std::make_shared<VR::VRInput>();

		bool VRactive;

		const unsigned int SCR_WIDTH = 1920;
		const unsigned int SCR_HEIGHT = 1080;
		GLuint m_nResolveFramebufferIdLeft;
		GLuint m_nResolveTextureIdLeft;

		GLuint m_nResolveFramebufferIdRight;
		GLuint m_nResolveTextureIdRight;

		bool activate();
		bool hasVR();
		void sumbitFrame();
		void handleInput(VRCameraController* cameraControler);

	protected:
		//

	private:
		std::shared_ptr<VRCore> VRCore;
	};
}
