#include "Utils.h"

long getTime() {
	SYSTEMTIME timeEnd;
	GetSystemTime(&timeEnd);
	return (timeEnd.wSecond * 1000) + timeEnd.wMilliseconds;
}
		 

