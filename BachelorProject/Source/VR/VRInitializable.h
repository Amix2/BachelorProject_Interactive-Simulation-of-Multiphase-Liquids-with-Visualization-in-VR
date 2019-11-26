#pragma once

namespace VR {
	/**
	 * The class implemented by all VR modules. since every VR module has to be somehow initialized.
	*/
	class VRInitializable
	{
	public:
		/**
		 * The public method to be called in order to initialize the module and mark it as initialized or not.
		 *
		 * @return Whether the operation has been completed successfully or not.
		*/
		bool Init();

		/**
		 * The method to be implemented in child classes. Implements the actual initialization of the module.
		 *
		 * @return Whether the operation has been completed successfully or not.
		*/
		virtual bool InitModule() = 0;

	protected:
		//

	private:
		/// Marker stating whether the module has been initialized or not.
		bool VrModuleInitialized = false;
	};
}
