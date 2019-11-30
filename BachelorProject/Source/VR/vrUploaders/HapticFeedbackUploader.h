#pragma once

#include <VR/vrUploaders/Uploader.h>
#include <VR/vrUploaders/vrUploadedData/UploadData.h>
#include <VR/vrUploaders/vrUploadedData/HapticFeedbackData.h>

namespace VR
{
	namespace Uploading
	{
		class HapticFeedbackUploader : Uploader<VR::UploadDataTypes::HapticFeedbackData>
		{
		public:
			HapticFeedbackUploader();

			bool Upload() override;

		protected:
			//

		private:
			//
		};
	}
}
