#include "ThreadManager.h"

std::vector<std::thread*>	g_threadIdsVector;
std::mutex	g_mutex_ThreadIdsVector;



void Threads::addThreadToList(std::thread* threadID)
{
	std::unique_lock<std::mutex> lck(g_mutex_ThreadIdsVector);
	
	g_threadIdsVector.push_back(threadID);

	lck.unlock();

	LOG_F(INFO, "New thread in list");
}

std::vector<std::thread*>::const_reverse_iterator  Threads::vecBegin()
{
	return g_threadIdsVector.rbegin();
}

std::vector<std::thread*>::const_reverse_iterator  Threads::vecEnd()
{
	return  g_threadIdsVector.rend();
}
