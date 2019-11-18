#pragma once

#include "Provider.h"

template <typename ProvidedDataType>
class VRCameraProvider final : Provider<ProvidedDataType>
{
public:
	VRCameraProvider();
	bool init() override;
	bool ReceiveData() override;
	bool IsReceivedDataStillValid() const override;

protected:
	//

private:
	//
};
