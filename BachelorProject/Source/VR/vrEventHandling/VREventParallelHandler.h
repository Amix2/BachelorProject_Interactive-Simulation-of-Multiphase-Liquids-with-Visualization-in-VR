#pragma once

namespace VR
{
	namespace EventHandling
	{
		template <typename CallbackReturnType = void, typename... CallbackArgumentTypes>
		class VREventParallelHandler
		{
		public:
			VREventParallelHandler();

			virtual CallbackReturnType RunParallel(CallbackArgumentTypes... CallbackArgumentTypes) = 0;
			bool HandleInParallel(CallbackArgumentTypes... CallbackArgumentTypes);
			CallbackReturnType GetResult();
			bool IsDone();

		protected:
			std::future<CallbackReturnType> Future;
			CallbackReturnType CallbackReturnedValue;
			bool IsDoneMarker{ false };

		private:
			//
		};
	}
}
