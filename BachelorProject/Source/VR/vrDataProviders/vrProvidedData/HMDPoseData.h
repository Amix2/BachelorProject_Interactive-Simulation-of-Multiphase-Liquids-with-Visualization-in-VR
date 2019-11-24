#pragma once

#include <glm/glm.hpp>

#include "ProvidedData.h"

namespace VR
{
	namespace ProvidedDataTypes
	{
		class HMDPoseData : ProvidedData
		{
		public:
			glm::mat4 HMDPose;

		protected:
			//

		private:
			//
		};
	}
}
