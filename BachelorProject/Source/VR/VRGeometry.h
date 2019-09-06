#pragma once

#include <array>
#include <map>

#include <OpenVR/openvr.h>

#include "utilities/Matrices.h"

namespace VR {
	class VRGeometry {
	public:
		VRGeometry(std::shared_ptr<vr::IVRSystem>);
		Matrix4 GetHMDMatrixProjectionEye(vr::Hmd_Eye);
		Matrix4 GetHMDMatrixPoseEye(vr::Hmd_Eye);
		Matrix4 GetCurrentViewProjectionMatrix(vr::Hmd_Eye);
		bool UpdateHMDMatrixPose();
		bool SetupCameras();
		unsigned int GetRenderHeight();
		unsigned int GetRenderWidth();

	protected:
		//

	private:
		Matrix4 SteamVRMatrixToMatrix4(const vr::HmdMatrix34_t&);

		vr::TrackedDevicePose_t TrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
		std::map<std::string, Matrix4> HmdMatrixProjectionEyes;
		std::map<std::string, Matrix4> HmdMatrixPoseEyes;
		Matrix4 HmdPose;
		std::shared_ptr<vr::IVRSystem> VrHandle;
		unsigned int ValidPosesCount;
		unsigned int RenderHeight;
		unsigned int RenderWidth;
		const float NearClip = 0.1f;
		const float FarClip = 30.03f;
	};
}
