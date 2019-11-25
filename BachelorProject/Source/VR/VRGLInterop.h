#pragma once

#include <GL/glew.h>
#include <memory>
#include <VR/InputConfig.h>
#include <VR/VRGeometry.h>
#include <VR/VRInput.h>
#include <VR/VRCore.h>
#include <scene/camera/VRCameraController.h>

namespace VR {
	class VRGLInterop {
	public:
		VRGLInterop(unsigned int width, unsigned int height) : width{ width }, height{ height } {};
		VRGLInterop& operator=(const VRGLInterop&) = default;
		std::shared_ptr<VR::VRCore> VrCore = std::make_unique<VR::VRCore>();
		std::shared_ptr<VR::VRGeometry> VrGeometry = std::make_unique<VR::VRGeometry>();
		std::shared_ptr<VR::VRInput> VrInput = std::make_unique<VR::VRInput>();

		bool VRactive;

		GLuint m_nResolveFramebufferIdLeft;
		GLuint m_nResolveTextureIdLeft;

		GLuint m_nResolveFramebufferIdRight;
		GLuint m_nResolveTextureIdRight;

		bool init();
		bool hasVR();
		void sumbitFrame(const FrameBuffer& frameBuffer);
		void handleInput(VRCameraController* cameraControler);

	protected:
		//

	private:
		unsigned int width;
		unsigned int height;

	};
};
