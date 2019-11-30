#include "HapticFeedbackUploader.h"

namespace VR
{
	namespace Uploading
	{
		HapticFeedbackUploader::HapticFeedbackUploader()
		{
			//
		}

		bool HapticFeedbackUploader::Upload()
		{
			this->VrSender->TriggerVibration(this->UploadData.ControllerIndex, this->UploadData.MicrosecondsDuration);
			return true;
		}
	}
}
