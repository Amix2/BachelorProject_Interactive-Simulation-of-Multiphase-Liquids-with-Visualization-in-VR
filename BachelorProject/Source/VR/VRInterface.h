#pragma once

#include <GL/glew.h>
#include <memory>
#include <VR/InputConfig.h>
#include <VR/VRGeometry.h>
#include <VR/VRInput.h>
#include <VR/VRCore.h>
#include <scene/camera/VRCameraController.h>

namespace VR {
	class VRInterface {
	public:
		VRInterface(unsigned int width, unsigned int height) : width{ width }, height{ height } {};
		VRInterface& operator=(const VRInterface&) = default;
		std::shared_ptr<VR::VRCore> VrCore = std::make_unique<VR::VRCore>();
		std::shared_ptr<VR::VRGeometry> VrGeometry = std::make_unique<VR::VRGeometry>();
		VR::VRInput* VrInput = new VR::VRInput(VrCore.get());

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
