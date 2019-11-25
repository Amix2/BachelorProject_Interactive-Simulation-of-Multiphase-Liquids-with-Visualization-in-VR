#pragma once

namespace VR
{
	namespace DataUploaders
	{
		template <typename UploadDataType>
		class Uploader
		{
		public:
			Uploader()
			{
				//
			}

			virtual bool UploadData() = 0;
			void SetData(UploadDataType UploadData);

		protected:
			std::shared_ptr<UploadData> UploadData;

		private:
			//
		};
	}
}
