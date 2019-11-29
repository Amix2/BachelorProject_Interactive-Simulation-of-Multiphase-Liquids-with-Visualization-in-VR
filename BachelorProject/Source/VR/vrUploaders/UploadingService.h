#pragma once

#include <VR/VRInitializable.h>
#include <VR/vrEventHandling/VREventParallelHandler.h>

namespace VR
{
	namespace Uploading
	{
		class UploadingService : VR::Implementation::VRInitializable, VR::EventHandling::VREventParallelHandler
		{
		public:
			UploadingService();

			bool UploadData();

		protected:
			//

		private:
			//
		};
	}
}
