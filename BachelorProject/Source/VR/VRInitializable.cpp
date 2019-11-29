#include "VRInitializable.h"

namespace VR
{
	namespace Implementation
	{
		bool VRInitializable::Init()
		{
			bool ModuleInitializationSuccessful = this->InitModule();
			this->VrModuleInitialized = ModuleInitializationSuccessful;
			return ModuleInitializationSuccessful;
		}
	}
}
