#pragma once

#include <memory>

#include <VR/VRSender.h>
#include <VR/vrUploaders/vrUploadedData/UploadData.h>

namespace VR
{
	namespace Uploading
	{
		template <typename UploadDataType>
		class Uploader abstract
		{
		public:
			Uploader();

			virtual bool Upload() = 0;
			void SetData(UploadDataType UploadData);

		protected:
			UploadDataType UploadData;
			std::shared_ptr<VR::Implementation::VRSender> VrSender{};

		private:
			//
		};
	}
}
