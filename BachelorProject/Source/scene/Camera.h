#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Scene {
	class Camera
	{
	public:
		Camera(glm::vec3& position, float pitch, float yaw, float rotation);

		glm::mat4 getViewMatrix() const;
		void set(glm::vec3 position, float pitch, float yaw, float roll);
		void set(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right);
	protected:
	private:
		static const glm::vec3 WORLD_UP;
		static const float DEFAULT_PITCH;
		static const float DEFAULT_YAW;
		static const float DEFAULT_ROLL;

		glm::vec3 headUp;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;

		float pitch;
		float yaw;
		float roll;

		void updateCameraVectors();
		void updateCameraAngles();
		};

	const glm::vec3 Camera::WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const float Camera::DEFAULT_PITCH{ 0.0f };
	const float Camera::DEFAULT_YAW{ 0.0f };
	const float Camera::DEFAULT_ROLL{ 0.0f };
}

#endif //CAMERA_H