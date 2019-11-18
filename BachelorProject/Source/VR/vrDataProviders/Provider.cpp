#include "Provider.h"

template <typename ProvidedDataType>
ProvidedDataType Provider<ProvidedDataType>::GetProvidedData() const
{
	return this->ProvidedData;
}
