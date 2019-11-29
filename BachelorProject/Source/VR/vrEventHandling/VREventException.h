#pragma once

#include <string>

#include <exception>

namespace VR
{
	namespace EventHandling
	{
		class VREventException : std::exception
		{
		public:
			explicit VREventException(const char* ExceptionMessage) : ExceptionMessage{ ExceptionMessage }
			{
				//
			}

			explicit VREventException(const std::string& ExceptionMessage) : ExceptionMessage{ ExceptionMessage }
			{
				//
			}

			virtual ~VREventException() throw()
			{
				//
			}

			virtual const char* what() const throw()
			{
				return this->ExceptionMessage.c_str();
			}

		protected:
			//

		private:
			std::string ExceptionMessage;
		};
	}
}
