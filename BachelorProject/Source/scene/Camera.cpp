#include "Camera.h"


namespace Scene {
	Camera::Camera(glm::vec3& position, float pitch, float yaw, float rotation)
		: position { position }
		, pitch{ pitch }
		, yaw{ yaw }
		, roll{ roll }
	{
		updateCameraVectors();
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::lookAt(position, position + front, up);
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
		pitch = asin(front.y / front.length())
		//TODO
	}
}
