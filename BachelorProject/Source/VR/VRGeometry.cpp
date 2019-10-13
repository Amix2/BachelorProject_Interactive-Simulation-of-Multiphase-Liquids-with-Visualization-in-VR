#include "VRGeometry.h"

namespace VR {
	VRGeometry::VRGeometry() {
		//
	}

	void VRGeometry::SetIVRSystem(vr::IVRSystem* ivrSystem) {
		this->VrHandle = ivrSystem;
		this->VrHandle->GetRecommendedRenderTargetSize(&RenderWidth, &RenderHeight);
	}

	vr::HmdMatrix44_t VRGeometry::GetHMDMatrixProjectionEye(vr::Hmd_Eye HmdEye) {
		if (!this->VrHandle) {
			return vr::HmdMatrix44_t(); // error
		}

		vr::HmdMatrix44_t HmdMatrix = this->VrHandle->GetProjectionMatrix(HmdEye, this->NearClip, this->FarClip);

		return HmdMatrix;
	}

	vr::HmdMatrix34_t VRGeometry::GetHMDMatrixPoseEye(vr::Hmd_Eye HmdEye) {
		if (!this->VrHandle) {
			return vr::HmdMatrix34_t(); // error
		}

		vr::HmdMatrix34_t HmdMatrix = this->VrHandle->GetEyeToHeadTransform(HmdEye);

		return HmdMatrix;
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