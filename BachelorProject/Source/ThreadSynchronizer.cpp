#include "ThreadSynchronizer.h"

void ThreadSynchronizer::lock()
{
	mutex.lock();
}

bool ThreadSynchronizer::tryLock()
{
	return mutex.try_lock();
}

void ThreadSynchronizer::unlock()
{
	mutex.unlock();
}

void ThreadSynchronizer::createDrawingObjectLock()
{
	lock();
	LOG_F(WARNING, "LOCK createDrawingObjectLock");
}

void ThreadSynchronizer::createDrawingObjectUnlock()
{
	LOG_F(WARNING, "UN-LOCK createDrawingObjectUnlock");
	unlock();
}

void ThreadSynchronizer::drawElementsLock()
{
	lock();
	LOG_F(WARNING, "LOCK drawElementsLock");
}

void ThreadSynchronizer::drawElementsUnlock()
{
	LOG_F(WARNING, "UN-LOCK drawElementsUnlock");
	unlock();
}

void ThreadSynchronizer::createComputeShaderLock()
{
	lock();
	LOG_F(WARNING, "LOCK createComputeShaderLock");
}

void ThreadSynchronizer::createComputeShaderUnlock()
{
	LOG_F(WARNING, "UN-LOCK createComputeShaderUnlock");
	unlock();
}

void ThreadSynchronizer::mapBufferLock()
{
	lock();
	LOG_F(WARNING, "LOCK mapBufferLock");
}

void ThreadSynchronizer::mapBufferUnlock()
{
	LOG_F(WARNING, "UN-LOCK mapBufferUnlock");
	unlock();
}

void ThreadSynchronizer::createBufferLock()
{
	lock();
	LOG_F(ERROR, "LOCK createBufferLock");
}

void ThreadSynchronizer::createBufferUnlock()
{
	LOG_F(ERROR, "UN-LOCK createBufferUnlock");
	unlock();
}
