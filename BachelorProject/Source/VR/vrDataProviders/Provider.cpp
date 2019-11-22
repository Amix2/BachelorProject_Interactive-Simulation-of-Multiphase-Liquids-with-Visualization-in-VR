#include "Provider.h"

namespace VR {
	namespace DataProviders {
		template <typename ProvidedDataType>
		ProvidedDataType Provider<ProvidedDataType>::GetProvidedData() const
		{
			return this->ProvidedData;
		}
	}
}
