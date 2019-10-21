#pragma once
#include <mutex>
#include <Logger.h>
//	!!!!!!!!!!!!!!!!!!
//	NOT USED
//	!!!!!!!!!!!!!!!!!!
class ThreadSynchronizer
{
private:
	inline static std::mutex mutex;
public:
	static void lock();
	static bool tryLock();
	static void unlock();

	static void createDrawingObjectLock();
	static void createDrawingObjectUnlock();

	static void drawElementsLock();
	static void drawElementsUnlock();

	static void createComputeShaderLock();
	static void createComputeShaderUnlock();

	static void mapBufferLock();
	static void mapBufferUnlock();

	static void createBufferLock();
	static void createBufferUnlock();

};

