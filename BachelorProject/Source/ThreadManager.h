#pragma once
#include <mutex>
#include <vector>
#include <Logger.h>

namespace Threads {

	void addThreadToList(std::thread* threadID);

	std::vector<std::thread*>::const_reverse_iterator  vecBegin();
	std::vector<std::thread*>::const_reverse_iterator  vecEnd();

}