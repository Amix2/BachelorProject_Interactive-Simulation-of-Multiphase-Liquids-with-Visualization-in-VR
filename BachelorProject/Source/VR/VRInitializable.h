#pragma once

namespace VR {
	class VRInitializable
	{
	public:
		bool init();
		virtual bool InitModule() = 0;

	protected:
		//

	private:
		bool VrModuleInitialized = false;
	};
}
