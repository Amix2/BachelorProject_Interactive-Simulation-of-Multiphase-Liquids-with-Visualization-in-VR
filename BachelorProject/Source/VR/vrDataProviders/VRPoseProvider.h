#pragma once

#include "Provider.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		class VRPoseProvider final : Provider<ProvidedDataType>
		{
		public:
			VRPoseProvider();
			bool init() override;
			bool ReceiveData() override;
			bool IsReceivedDataStillValid() const override;

		protected:
			//

		private:
			//
		};
	}
}