#pragma once

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
			ControllerData(ControllerInputData ControllerInputData, ControllerPoseData ControllerPoseData) : ControllerInputData{ ControllerInputData }, ControllerPoseData{ ControllerPoseData }
			{
				//
			}
			ControllerInputData ControllerInputData;
			ControllerPoseData ControllerPoseData;

		protected:
			//

		private:
			//
		};
	}
}
