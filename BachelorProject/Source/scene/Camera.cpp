#include "Camera.h"
#include <iostream>
namespace Scene {
	Camera::Camera(ViewPort& viewPort, const glm::vec3& position, float pitch, float yaw, float roll)
		: viewPort{ &viewPort }
		, position { position }
		, pitch{ pitch }
		, yaw{ yaw }
		, roll{ roll }
	{
		updateCameraVectors();
	}

	Camera::Camera(ViewPort& viewPort, const glm::vec3& position)
		: viewPort{ &viewPort }
		, position{ position }
		, pitch{ DEFAULT_CAM_PITCH }
		, yaw{ DEFAULT_CAM_YAW }
		, roll{ DEFAULT_CAM_ROLL }
	{
		updateCameraVectors();
	}


	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::lookAt(position, position + front, up);
	}

	void Camera::select() const
	{
		glViewport(viewPort->getX(), viewPort->getY(), viewPort->getWidth(), viewPort->getHeight());
	}


	void Camera::set(glm::vec3 position, float pitch, float yaw, float roll)
	{
		this->position = position;
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = roll;
		updateCameraVectors();
	}

	void Camera::set(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right) {
		this->position = position;
		this->front = front;
		this->up = up;
		this->right = right;
		updateCameraAngles();
	}

	void Camera::set(const vr::HmdMatrix34_t& data)
	{
		right = -glm::vec3{ data.m[0][0], data.m[1][0], data.m[2][0] };
		up = glm::vec3{ data.m[0][1], data.m[1][1], data.m[2][1] };
		front = glm::vec3{ data.m[0][2], data.m[1][2], data.m[2][2] };
		position = glm::vec3{ data.m[0][3], data.m[1][3], data.m[2][3] };
		updateCameraAngles();

	}

	void Camera::updateCameraVectors() {
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(newFront);

		headUp = glm::normalize(glm::rotate(WORLD_UP, glm::radians(roll), front));

		right = glm::normalize(glm::cross(front, headUp));

		up = glm::normalize(glm::cross(right, front));
	}

	void Camera::updateCameraAngles() {
		//TODO
	}
}
