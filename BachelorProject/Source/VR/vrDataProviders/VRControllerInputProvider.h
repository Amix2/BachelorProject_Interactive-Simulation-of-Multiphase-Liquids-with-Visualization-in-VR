#pragma once

#include "Provider.h"

template <typename ProvidedDataType>
class VRControllerInputProvider final : Provider<ProvidedDataType>
{
public:
	bool init() override;
	bool ReceiveData() override;
	bool IsReceivedDataStillValid() const override;

protected:
	//

private:
	//
};
