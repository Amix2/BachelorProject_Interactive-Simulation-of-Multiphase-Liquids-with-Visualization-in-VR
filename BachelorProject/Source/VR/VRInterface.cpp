#include "VRInterface.h"

namespace VR {
	bool VRInterface::init() {
		VRactive = VrCore->InitializeCore();
		if (!VRactive) return false;
		//VrInput->InitializeVRInput(std::string(VR::ACTIONS_PATH));
		VrGeometry->SetIVRSystem(VrCore->GetVrSystem());
		////////////////////

		glGenFramebuffers(1, &m_nResolveFramebufferIdLeft);
		glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

		glGenTextures(1, &m_nResolveTextureIdLeft);
		glBindTexture(GL_TEXTURE_2D, m_nResolveTextureIdLeft);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nResolveTextureIdLeft, 0);

		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		///////////////////////

		////////////////////

		glGenFramebuffers(1, &m_nResolveFramebufferIdRight);
		glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdRight);

		glGenTextures(1, &m_nResolveTextureIdRight);
		glBindTexture(GL_TEXTURE_2D, m_nResolveTextureIdRight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nResolveTextureIdRight, 0);

		// check FBO status
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		///////////////////////

		return true;
	}

	bool VRInterface::hasVR()
	{
		return VRactive;
	}

	void VRInterface::sumbitFrame(const FrameBuffer& frameBuffer) {

		frameBuffer.readFrom();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

		glBlitFramebuffer(0, 0, frameBuffer.getWidth() * 0.5f, frameBuffer.getHeight(), 0, 0, frameBuffer.getWidth(), frameBuffer.getHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_nResolveFramebufferIdRight);

		glBlitFramebuffer(frameBuffer.getWidth() * 0.5f, 0, frameBuffer.getWidth(), frameBuffer.getHeight(), 0, 0, frameBuffer.getWidth(), frameBuffer.getHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		auto [LeftTexture, RightTexture] = VrGeometry->ObtainTextures(m_nResolveTextureIdLeft, m_nResolveTextureIdRight);
		VrCore->SubmitTexturesToHMD(LeftTexture, RightTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void VRInterface::handleInput(VRCameraController* cameraController) {

		VrGeometry->UpdateHMDMatrixPose();
		VrGeometry->SetupCameras();
		//VrInput->DetectPressedButtons();
		//VrInput->HandleInput();
		cameraController->setHead(VrGeometry->TrackedDevicePoses[0].mDeviceToAbsoluteTracking);
		cameraController->setEyeMatrix(VrGeometry->GetHMDMatrixPoseEye(vr::Eye_Left), vr::Eye_Left);
		cameraController->setEyeMatrix(VrGeometry->GetHMDMatrixPoseEye(vr::Eye_Right), vr::Eye_Right);
		cameraController->setProjectionMatrix(VrGeometry->GetHMDMatrixProjectionEye(vr::Eye_Left), vr::Eye_Left);
		cameraController->setProjectionMatrix(VrGeometry->GetHMDMatrixProjectionEye(vr::Eye_Right), vr::Eye_Right);
	}
}
