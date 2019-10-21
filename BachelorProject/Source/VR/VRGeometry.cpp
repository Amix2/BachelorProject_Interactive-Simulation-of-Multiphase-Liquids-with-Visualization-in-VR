#include "VRGeometry.h"

#include <glm/glm.hpp>

#include <utilities/Matrices.h>

namespace VR {
	VRGeometry::VRGeometry() {
		//
	}

	void VRGeometry::SetIVRSystem(vr::IVRSystem* ivrSystem) {
		this->VrHandle = ivrSystem;
		this->VrHandle->GetRecommendedRenderTargetSize(&RenderWidth, &RenderHeight);
	}

	glm::mat4 VRGeometry::GetHMDMatrixProjectionEye(vr::Hmd_Eye HmdEye) const {
		if (!this->VrHandle) {
			return glm::mat4(); // error
		}

		vr::HmdMatrix44_t HmdMatrix4x4 = this->VrHandle->GetProjectionMatrix(HmdEye, this->NearClip, this->FarClip);
		glm::mat4 GlmHmdMatrix4x4(
			HmdMatrix4x4.m[0][0], HmdMatrix4x4.m[1][0], HmdMatrix4x4.m[2][0], HmdMatrix4x4.m[3][0],
			HmdMatrix4x4.m[0][1], HmdMatrix4x4.m[1][1], HmdMatrix4x4.m[2][1], HmdMatrix4x4.m[3][1],
			HmdMatrix4x4.m[0][2], HmdMatrix4x4.m[1][2], HmdMatrix4x4.m[2][2], HmdMatrix4x4.m[3][2],
			HmdMatrix4x4.m[0][3], HmdMatrix4x4.m[1][3], HmdMatrix4x4.m[2][3], HmdMatrix4x4.m[3][3]
		);

		return GlmHmdMatrix4x4;
	}

	vr::HmdMatrix34_t VRGeometry::GetHMDMatrixPoseEye(vr::Hmd_Eye HmdEye) const {
		if (!this->VrHandle) {
			return vr::HmdMatrix34_t(); // error
		}

		vr::HmdMatrix34_t HmdMatrix3x4 = this->VrHandle->GetEyeToHeadTransform(HmdEye);
		/*
		Matrix4 TemporaryMatrix3x4(
			HmdMatrix3x4.m[0][0], HmdMatrix3x4.m[1][0], HmdMatrix3x4.m[2][0], 0,
			HmdMatrix3x4.m[0][1], HmdMatrix3x4.m[1][1], HmdMatrix3x4.m[2][1], 0,
			HmdMatrix3x4.m[0][2], HmdMatrix3x4.m[1][2], HmdMatrix3x4.m[2][2], 0,
			HmdMatrix3x4.m[0][3], HmdMatrix3x4.m[1][3], HmdMatrix3x4.m[2][3], 1
		);
		TemporaryMatrix3x4.invert();
		const float* MemoryFromMatrix = TemporaryMatrix3x4.get();
		vr::HmdMatrix34_t InvertedMatrix3x4(
			MemoryFromMatrix[0], MemoryFromMatrix[1], MemoryFromMatrix[2], MemoryFromMatrix[3],
			MemoryFromMatrix[4], MemoryFromMatrix[5], MemoryFromMatrix[6], MemoryFromMatrix[7],
			MemoryFromMatrix[8], MemoryFromMatrix[9], MemoryFromMatrix[10], MemoryFromMatrix[11],
			MemoryFromMatrix[12], MemoryFromMatrix[13], MemoryFromMatrix[14], MemoryFromMatrix[15]
		);
		*/

		return HmdMatrix3x4;
	}

	bool VRGeometry::UpdateHMDMatrixPose() {
		if (!this->VrHandle) {
			return false; // error
		}

		//vr::VRCompositor()->WaitGetPoses(this->TrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
		this->VrHandle->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, &this->TrackedDevicePose, 1);

		/*
		ValidPosesCount = 0;
		for (unsigned int Device = 0; Device < vr::k_unMaxTrackedDeviceCount; ++Device) {
			if (this->TrackedDevicePose[Device].bPoseIsValid) {
				++ValidPosesCount;
			}
		}

		if (this->TrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
			this->HmdPose = SteamVRMatrixToMatrix4(this->TrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
			this->HmdPose.invert();
		}
		*/

		return true;
	}

	bool VRGeometry::SetupCameras() {
		this->HmdMatrixProjectionEyes["left"] = GetHMDMatrixProjectionEye(vr::Eye_Left);
		this->HmdMatrixProjectionEyes["right"] = GetHMDMatrixProjectionEye(vr::Eye_Right);
		this->HmdMatrixPoseEyes["left"] = GetHMDMatrixPoseEye(vr::Eye_Left);
		this->HmdMatrixPoseEyes["right"] = GetHMDMatrixPoseEye(vr::Eye_Right);

		return true;
	}

	unsigned int VRGeometry::GetRenderHeight() {
		return this->RenderHeight;
	}

	unsigned int VRGeometry::GetRenderWidth() {
		return this->RenderWidth;
	}

	std::tuple<vr::Texture_t, vr::Texture_t> VRGeometry::ObtainTextures(GLuint LeftResolveTextureId, GLuint RightResolveTextureId) {
		std::tuple<vr::Texture_t, vr::Texture_t> Textures;
		auto ColorSpace = this->IsColorSpaceLinear ? vr::ColorSpace_Linear : vr::ColorSpace_Gamma;
		vr::Texture_t LeftTexture = {(void *) (uintptr_t) LeftResolveTextureId, vr::TextureType_OpenGL, ColorSpace};
		vr::Texture_t RightTexture = {(void*) (uintptr_t) RightResolveTextureId, vr::TextureType_OpenGL, ColorSpace};
		std::get<0>(Textures) = LeftTexture;
		std::get<1>(Textures) = RightTexture;

		return Textures;
	}

	Matrix4 VRGeometry::SteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& VrMatrix) {
		return Matrix4(
			VrMatrix.m[0][0], VrMatrix.m[1][0], VrMatrix.m[2][0], 0.0,
			VrMatrix.m[0][1], VrMatrix.m[1][1], VrMatrix.m[2][1], 0.0,
			VrMatrix.m[0][2], VrMatrix.m[1][2], VrMatrix.m[2][2], 0.0,
			VrMatrix.m[0][3], VrMatrix.m[1][3], VrMatrix.m[2][3], 1.0f
		);
	}
}