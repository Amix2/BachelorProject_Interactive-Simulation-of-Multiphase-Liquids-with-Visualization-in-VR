#include <future>
#include <functional>

#include "VREventParallelHandler.h"

namespace VR
{
	namespace EventHandling
	{
		template <>
		VREventParallelHandler<>::VREventParallelHandler()
		{
			//
		}

		template <typename CallbackReturnType, typename ...CallbackArgumentTypes>
		bool VREventParallelHandler<CallbackReturnType, CallbackArgumentTypes...>::HandleInParallel(CallbackArgumentTypes... CallbackArguments)
		{
			this->Future = std::async(std::launch::async, FunctionToRunInParallel, CallbackArguments...);
			this->IsDoneMarker = false;
		}

		template <typename CallbackReturnType, typename ...CallbackArgumentTypes>
		inline CallbackReturnType VREventParallelHandler<CallbackReturnType, CallbackArgumentTypes...>::GetResult()
		{
			this->CallbackReturnedValue = this->Future.get();
			this->IsDoneMarker = true;
			return this->CallbackReturnedValue;
		}

		template <>
		inline bool VREventParallelHandler<>::IsDone()
		{
			return this->IsDoneMarker;
		}
	}
}
