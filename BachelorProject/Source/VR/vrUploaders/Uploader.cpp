#include "Uploader.h"

namespace VR
{
	namespace Uploading
	{
		template <typename UploadDataType>
		Uploader<UploadDataType>::Uploader()
		{
			//
		}

		template<typename UploadDataType>
		void Uploader<UploadDataType>::SetData(UploadDataType UploadData)
		{
			this->UploadData = UploadData;
		}
	}
}
