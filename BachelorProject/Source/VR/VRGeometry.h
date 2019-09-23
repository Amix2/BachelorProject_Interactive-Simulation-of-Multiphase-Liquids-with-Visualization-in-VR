#pragma once

#include <array>
#include <map>

#include <OpenVR/openvr.h>

#include "utilities/Matrices.h"

namespace VR {
	class VRGeometry {
	public:
		VRGeometry();
		void SetIVRSystem(vr::IVRSystem*);
		vr::HmdMatrix44_t GetHMDMatrixProjectionEye(vr::Hmd_Eye);
		vr::HmdMatrix34_t GetHMDMatrixPoseEye(vr::Hmd_Eye);
		bool UpdateHMDMatrixPose();
		bool SetupCameras();
		unsigned int GetRenderHeight();
		unsigned int GetRenderWidth();
		vr::TrackedDevicePose_t TrackedDevicePose[vr::k_unMaxTrackedDeviceCount];

	protected:
		//

	private:
		Matrix4 SteamVRMatrixToMatrix4(const vr::HmdMatrix34_t&);

		std::map<std::string, vr::HmdMatrix44_t> HmdMatrixProjectionEyes;
		std::map<std::string, vr::HmdMatrix34_t> HmdMatrixPoseEyes;
		Matrix4 HmdPose;
		vr::IVRSystem * VrHandle;
		unsigned int ValidPosesCount;
		unsigned int RenderHeight;
		unsigned int RenderWidth;
		const float NearClip = 0.1f;
		const float FarClip = 30.03f;
	};
}
