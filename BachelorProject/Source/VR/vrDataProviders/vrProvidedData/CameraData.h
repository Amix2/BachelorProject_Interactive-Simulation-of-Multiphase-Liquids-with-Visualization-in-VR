#pragma once

#include <OpenVR/openvr.h>

#include "ProvidedData.h"

namespace VR
{
	namespace ProvidedDataTypes
	{
		class CameraData : ProvidedData
		{
		public:
			CameraData()
			{
				//
			}

			void SetCameraHeight(unsigned int CameraHeight)
			{
				this->CameraHeight = CameraHeight;
			}

			void SetCameraWidth(unsigned int CameraWidth)
			{
				this->CameraWidth = CameraWidth;
			}

			unsigned int CameraHeight;
			unsigned int CameraWidth;

		protected:
			//

		private:
			//
		};
	}
}
