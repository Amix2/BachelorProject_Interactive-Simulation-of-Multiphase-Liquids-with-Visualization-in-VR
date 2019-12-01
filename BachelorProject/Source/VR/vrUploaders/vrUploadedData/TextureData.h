#pragma once

#include <OpenVR/openvr.h>

#include <VR/VRCore.h>
#include <VR/vrUploaders/vrUploadedData/UploadData.h>

namespace VR
{
	namespace UploadDataTypes
	{
		class TextureData : public UploadData
		{
		public:
			TextureData(vr::Texture_t LeftTexture, vr::Texture_t RightTexture) : LeftTexture{ LeftTexture }, RightTexture{ RightTexture }
			{
				//
			}
			vr::Texture_t LeftTexture;
			vr::Texture_t RightTexture;

		protected:
			//

		private:
			//
		};
	}
}
