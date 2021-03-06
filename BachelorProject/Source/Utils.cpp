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

glm::vec3 Utils::getPosition(const glm::mat4 matrix)
{
	return  glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
}

void Utils::setUp(glm::mat4* matrix, const glm::vec3& vector)
{
	matrix->operator[](1)[0] = vector.x;
	matrix->operator[](1)[1] = vector.y;
	matrix->operator[](1)[2] = vector.z;
}

void Utils::setRight(glm::mat4* matrix, const glm::vec3& vector)
{
	matrix->operator[](0)[0] = vector.x;
	matrix->operator[](0)[1] = vector.y;
	matrix->operator[](0)[2] = vector.z;
}

void Utils::setForward(glm::mat4* matrix, const glm::vec3& vector)
{
	matrix->operator[](2)[0] = vector.x;
	matrix->operator[](2)[1] = vector.y;
	matrix->operator[](2)[2] = vector.z;
}

void Utils::setPosition(glm::mat4* matrix, const glm::vec3& vector)
{
	matrix->operator[](3)[0] = vector.x;
	matrix->operator[](3)[1] = vector.y;
	matrix->operator[](3)[2] = vector.z;
}

glm::mat4 Utils::getRotationMatrix(glm::vec3 axis, float angle)
{
	axis = normalize(axis);
	float s = std::sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;

	return glm::mat4(
		glm::vec4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0),
		glm::vec4(oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0),
		glm::vec4(oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0));
}
		 

