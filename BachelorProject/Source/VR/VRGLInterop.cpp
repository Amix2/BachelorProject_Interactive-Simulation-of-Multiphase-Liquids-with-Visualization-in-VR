#include "VRGLInterop.h"

namespace VR {
	bool VRGLInterop::activate() {
		VRactive = VrCore->InitializeCore();
		if (!VRactive) return false;
		////////////////////

		glGenFramebuffers(1, &m_nResolveFramebufferIdLeft);
		glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

		glGenFramebuffers(1, &m_nRenderFramebufferIdLeft);
		glBindFramebuffer(GL_FRAMEBUFFER, m_nRenderFramebufferIdLeft);

		glGenRenderbuffers(1, &m_nDepthBufferIdLeft);
		glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthBufferIdLeft);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthBufferIdLeft);

		glGenTextures(1, &m_nRenderTextureIdLeft);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdLeft);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, true);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdLeft, 0);

		glGenFramebuffers(1, &m_nResolveFramebufferIdLeft);
		glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

		glGenTextures(1, &m_nResolveTextureIdLeft);
		glBindTexture(GL_TEXTURE_2D, m_nResolveTextureIdLeft);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

		glGenFramebuffers(1, &m_nRenderFramebufferIdRight);
		glBindFramebuffer(GL_FRAMEBUFFER, m_nRenderFramebufferIdRight);

		glGenRenderbuffers(1, &m_nDepthBufferIdRight);
		glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthBufferIdRight);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthBufferIdRight);

		glGenTextures(1, &m_nRenderTextureIdRight);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdRight);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, true);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdRight, 0);

		glGenFramebuffers(1, &m_nResolveFramebufferIdRight);
		glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdRight);

		glGenTextures(1, &m_nResolveTextureIdRight);
		glBindTexture(GL_TEXTURE_2D, m_nResolveTextureIdRight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

	bool VRGLInterop::hasVR()
	{
		return VRactive;
	}

	void VRGLInterop::phase1() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_nRenderFramebufferIdLeft);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void VRGLInterop::phase2(VRCameraController* cameraController) {

		VrGeometry->SetupCameras();
		VrGeometry->UpdateHMDMatrixPose();
		VrInput->DetectPressedButtons();
		VrInput->HandleInput();
		cameraController->setHead(VrGeometry->TrackedDevicePose.mDeviceToAbsoluteTracking);
		auto [LeftTexture, RightTexture] = VrGeometry->ObtainTextures(m_nResolveTextureIdLeft, m_nResolveTextureIdRight);
		vr::TrackedDevicePose_t xxx[vr::k_unMaxTrackedDeviceCount];
		std::cout << vr::VRCompositor()->WaitGetPoses(xxx, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
		VrCore->SubmitTexturesToHMD(LeftTexture, RightTexture);
	}
}
