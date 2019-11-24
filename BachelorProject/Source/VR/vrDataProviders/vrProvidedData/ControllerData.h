#pragma once

#include <memory>

#include "ProvidedData.h"
#include "ControllerInputData.h"
#include "ControllerPoseData.h"

namespace VR
{
	namespace ProvidedDataTypes
	{
		struct ControllerData : ProvidedData
		{
		public:
			std::shared_ptr<ControllerInputData> ControllerInputData;
			std::shared_ptr<ControllerPoseData> ControllerPoseData;

		protected:
			//

		private:
			//
		};
	}
}
