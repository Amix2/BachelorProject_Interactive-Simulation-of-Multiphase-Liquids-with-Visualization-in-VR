#include "VRInitializable.h"

namespace VR
{
	bool VRInitializable::init()
	{
		bool ModuleInitializationSuccessful = this->InitModule();
		this->VrModuleInitialized = ModuleInitializationSuccessful;
		return ModuleInitializationSuccessful;
	}
}
