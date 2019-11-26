#include "VRInitializable.h"

namespace VR
{
	bool VRInitializable::Init()
	{
		bool ModuleInitializationSuccessful = this->InitModule();
		this->VrModuleInitialized = ModuleInitializationSuccessful;
		return ModuleInitializationSuccessful;
	}
}
