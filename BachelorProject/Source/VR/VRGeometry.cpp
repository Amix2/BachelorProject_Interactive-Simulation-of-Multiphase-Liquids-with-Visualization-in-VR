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

	glm::mat4 openvr_m34_to_mat4(const vr::HmdMatrix34_t& t) {
		return glm::mat4(
			t.m[0][0], t.m[1][0], t.m[2][0], 0.0f,
			t.m[0][1], t.m[1][1], t.m[2][1], 0.0f,
			t.m[0][2], t.m[1][2], t.m[2][2], 0.0f,
			t.m[0][3], t.m[1][3], t.m[2][3], 1.0f
		);
	}

	glm::mat4 openvr_m44_to_mat4(const vr::HmdMatrix44_t& t) {
		return glm::mat4(
			t.m[0][0], t.m[1][0], t.m[2][0], t.m[3][0],
			t.m[0][1], t.m[1][1], t.m[2][1], t.m[3][1],
			t.m[0][2], t.m[1][2], t.m[2][2], t.m[3][2],
			t.m[0][3], t.m[1][3], t.m[2][3], t.m[3][3]
		);
	}

	glm::mat4 VRGeometry::GetHMDMatrixProjectionEye(vr::Hmd_Eye HmdEye) const {
		if (!this->VrHandle) {
			std::cout << "Bad P" << std::endl;
			return glm::mat4(); // error
		}

		vr::HmdMatrix44_t HmdMatrix4x4 = this->VrHandle->GetProjectionMatrix(HmdEye, this->NearClip, this->FarClip);
		glm::mat4 GlmProjectionMatrix4x4 = openvr_m44_to_mat4(HmdMatrix4x4);

		return GlmProjectionMatrix4x4;
	}

	glm::mat4 VRGeometry::GetHMDMatrixPoseEye(vr::Hmd_Eye HmdEye) const {
		if (this->VrHandle == nullptr) {
			std::cout << "Bad E" << std::endl;
			return glm::mat4(); // error
		}

		vr::HmdMatrix34_t HmdMatrix3x4 = this->VrHandle->GetEyeToHeadTransform(HmdEye);
		
		glm::mat4 GlmEyeMatrix4x4 = openvr_m34_to_mat4(HmdMatrix3x4);

		return glm::inverse(GlmEyeMatrix4x4);
	}

	bool VRGeometry::UpdateHMDMatrixPose() {
		if (this->VrHandle == nullptr) {
			return false; // error
		}

		vr::VRCompositor()->WaitGetPoses(this->TrackedDevicePoses, vr::k_unMaxTrackedDeviceCount, NULL, 0);

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