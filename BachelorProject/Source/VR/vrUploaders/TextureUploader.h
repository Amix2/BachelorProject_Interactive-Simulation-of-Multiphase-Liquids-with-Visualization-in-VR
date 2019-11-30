#pragma once

#include <VR/vrUploaders/vrUploadedData/TextureData.h>
#include <VR/vrUploaders/Uploader.h>

namespace VR
{
	namespace Uploading
	{
		class TextureUploader : Uploader<VR::UploadDataTypes::TextureData>
		{
		public:
			TextureUploader();

			bool Upload() override;

		protected:
			//

		private:
			//
		};
	}
}
