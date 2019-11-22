#include "Utils.h"

long Utils::getTime() {
	SYSTEMTIME timeEnd;
	GetSystemTime(&timeEnd);
	return (timeEnd.wSecond * 1000) + timeEnd.wMilliseconds;
}

std::chrono::time_point<std::chrono::steady_clock> Utils::getNanoTime()
{
	return std::chrono::high_resolution_clock::now();
}

double Utils::getNanoTimeDif(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> finish)
{
	return ((double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count()) / 1000000;
}

glm::vec3 Utils::getUp(const glm::mat4 matrix)
{
	return glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]);
}

glm::vec3 Utils::getRight(const glm::mat4 matrix)
{
	return glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]);
}

glm::vec3 Utils::getForward(const glm::mat4 matrix)
{
	return glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]);
}

glm::vec3 Utils::getPosition(glm::mat4 matrix)
{
	return  glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
}
		 

