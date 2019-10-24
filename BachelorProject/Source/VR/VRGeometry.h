#pragma once

#include <array>
#include <map>
#include <tuple>

#include <OpenVR/openvr.h>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "utilities/Matrices.h"

namespace VR {
	class VRGeometry {
	public:
		VRGeometry();
		void SetIVRSystem(vr::IVRSystem*);
		glm::mat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye) const;
		glm::mat4 GetHMDMatrixPoseEye(vr::Hmd_Eye) const;
		bool UpdateHMDMatrixPose();
		bool SetupCameras();
		unsigned int GetRenderHeight();
		unsigned int GetRenderWidth();
		vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
		std::tuple<vr::Texture_t, vr::Texture_t> ObtainTextures(GLuint LeftResolveTextureId, GLuint RightResolveTextureId);

	protected:
		//

	private:
		const bool IsColorSpaceLinear = false;

		Matrix4 SteamVRMatrixToMatrix4(const vr::HmdMatrix34_t&);

		std::map<std::string, glm::mat4> HmdMatrixProjectionEyes;
		std::map<std::string, glm::mat4> HmdMatrixPoseEyes;
		Matrix4 HmdPose;
		vr::IVRSystem * VrHandle;
		unsigned int ValidPosesCount;
		unsigned int RenderHeight;
		unsigned int RenderWidth;
		const float NearClip = 0.1f;
		const float FarClip = 200.00f;
	};
}
