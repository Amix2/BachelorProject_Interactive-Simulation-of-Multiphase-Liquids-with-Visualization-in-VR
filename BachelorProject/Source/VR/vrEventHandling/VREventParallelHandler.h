#pragma once

namespace VR
{
	namespace EventHandling
	{
		template <typename ParallellyHandledFunction>
		class VREventParallelHandler
		{
		public:
			VREventParallelHandler()
			{
				//
			}

			bool HandleInParallel(ParallellyHandledFunction Function);

		protected:
			//

		private:
			//
		};
	}
}
