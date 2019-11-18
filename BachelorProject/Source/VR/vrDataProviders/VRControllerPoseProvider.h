#pragma once

#include <OpenVR/openvr.h>

#include "Provider.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		class VRControllerPoseProvider final : Provider<ProvidedDataType>
		{
		public:
			vr::ETrackedControllerRole ControllerRole;
			VRControllerPoseProvider();
			bool init() override;
			bool ReceiveData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			bool ReceiveControllerPose
		};
	}
}
