#include "TextureUploader.h"

namespace VR
{
	namespace Uploading
	{
		TextureUploader::TextureUploader()
		{
			//
		}

		bool TextureUploader::Upload()
		{
			this->VrSender->SubmitTexturesToHMD(this->UploadData.LeftTexture, this->UploadData.RightTexture);
		}
	}
}
