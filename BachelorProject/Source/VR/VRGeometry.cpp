#include "VRGeometry.h"

namespace VR {
	VRGeometry::VRGeometry(std::shared_ptr<vr::IVRSystem> VrSystem ) {
		this->VrHandle = VrSystem;
		this->VrHandle->GetRecommendedRenderTargetSize(&RenderWidth, &RenderHeight);
	}

	Matrix4 VRGeometry::GetHMDMatrixProjectionEye(vr::Hmd_Eye HmdEye) {
		if (!this->VrHandle) {
			return Matrix4(); // error
		}

		vr::HmdMatrix44_t HmdMatrix = this->VrHandle->GetProjectionMatrix(HmdEye, this->NearClip, this->FarClip);

		return Matrix4(
			HmdMatrix.m[0][0], HmdMatrix.m[1][0], HmdMatrix.m[2][0], HmdMatrix.m[3][0],
			HmdMatrix.m[0][1], HmdMatrix.m[1][1], HmdMatrix.m[2][1], HmdMatrix.m[3][1],
			HmdMatrix.m[0][2], HmdMatrix.m[1][2], HmdMatrix.m[2][2], HmdMatrix.m[3][2],
			HmdMatrix.m[0][3], HmdMatrix.m[1][3], HmdMatrix.m[2][3], HmdMatrix.m[3][3]
		);
	}

	Matrix4 VRGeometry::GetHMDMatrixPoseEye(vr::Hmd_Eye HmdEye) {
		if (!this->VrHandle) {
			return Matrix4(); // error
		}

		vr::HmdMatrix34_t HmdMatrix = this->VrHandle->GetEyeToHeadTransform(HmdEye);

		Matrix4 Matrix = SteamVRMatrixToMatrix4(HmdMatrix);

		return Matrix;
	}

	Matrix4 VRGeometry::GetCurrentViewProjectionMatrix(vr::Hmd_Eye HmdEye) {
		Matrix4 matMVP;
		if (HmdEye == vr::Eye_Left) {
			matMVP = this->HmdMatrixProjectionEyes["left"] * this->HmdMatrixPoseEyes["left"] * this->HmdPose;
		}
		else if (HmdEye == vr::Eye_Right) {
			matMVP = this->HmdMatrixProjectionEyes["right"] * this->HmdMatrixPoseEyes["right"] * this->HmdPose;
		}

		return matMVP;
	}

	bool VRGeometry::UpdateHMDMatrixPose() {
		if (!this->VrHandle) {
			return false; // error
		}

		vr::VRCompositor()->WaitGetPoses(this->TrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

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

	Matrix4 VRGeometry::SteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& VrMatrix) {
		return Matrix4(
			VrMatrix.m[0][0], VrMatrix.m[1][0], VrMatrix.m[2][0], 0.0,
			VrMatrix.m[0][1], VrMatrix.m[1][1], VrMatrix.m[2][1], 0.0,
			VrMatrix.m[0][2], VrMatrix.m[1][2], VrMatrix.m[2][2], 0.0,
			VrMatrix.m[0][3], VrMatrix.m[1][3], VrMatrix.m[2][3], 1.0f
		);
	}
}