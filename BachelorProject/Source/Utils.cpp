#include "Utils.h"

long getTime() {
	SYSTEMTIME timeEnd;
	GetSystemTime(&timeEnd);
	return (timeEnd.wSecond * 1000) + timeEnd.wMilliseconds;
}

std::chrono::time_point<std::chrono::steady_clock> getNanoTime()
{
	return std::chrono::high_resolution_clock::now();
}

double getNanoTimeDif(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> finish)
{
	return ((double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count()) / 1000000;
}
		 

