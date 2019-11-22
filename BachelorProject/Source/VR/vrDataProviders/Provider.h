#pragma once

#include "vrProvidedData/ProvidedData.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType = VR::DataProviders::ProvidedData>
		class Provider {
		public:
			virtual bool init() = 0;
			ProvidedDataType GetProvidedData() const;
			virtual bool ReceiveData() = 0;
			virtual bool IsReceivedDataStillValid() const = 0;

		protected:
			//

		private:
			ProvidedDataType ProvidedData;
		};
	}
}