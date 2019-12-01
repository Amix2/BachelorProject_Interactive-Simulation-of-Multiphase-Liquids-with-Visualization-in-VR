#pragma once

#include "vrProvidedData/ProvidedData.h"

namespace VR
{
	namespace DataProviders
	{
		template <typename ProvidedDataType>
		class Provider abstract {
		public:
			ProvidedDataType GetProvidedData() const;

		protected:
			ProvidedDataType ProvidedData;

		private:
			//
		};
	}
}