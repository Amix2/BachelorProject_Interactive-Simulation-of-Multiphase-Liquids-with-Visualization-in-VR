#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "ViewPort.h"
#include <OpenVR/openvr.h>
#include <utility>

namespace Scene {
	class Camera
	{
	public:
		Camera(ViewPort& viewPort, const glm::vec3& position, float pitch, float yaw, float roll);
		Camera(ViewPort& viewPort, const glm::vec3& position);

		glm::mat4 getViewMatrix() const;
		void select() const;
		void set(glm::vec3 position, float pitch, float yaw, float roll);
		void set(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right);
		void set(const vr::HmdMatrix44_t& data);

		void setRotation(float pitch, float yaw, float roll);
		void setPosition(const glm::vec3& position);

		const glm::vec3 getPosition() const { return position; };
		const glm::vec3 getFront() const { return front; };
		const glm::vec3 getRight() const { return right; };
		const glm::vec3 getUp() const { return up; };
		const float& getPitch() const { return pitch; };
		const float& getYaw() const { return yaw; };
		const float& getRoll() const { return roll; };
		
	protected:
	private:
		inline static const glm::vec3 WORLD_UP{ 0.0f, 1.0f, 0.0f };
		inline static const float DEFAULT_CAM_PITCH{ 0.0f };
		inline static const float DEFAULT_CAM_YAW{ 0.0f };
		inline static const float DEFAULT_CAM_ROLL{ 0.0f };

		glm::vec3 headUp;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;

		float pitch;
		float yaw;
		float roll;

		ViewPort* viewPort;

		void updateCameraVectors();
		void updateCameraAngles();
		};
}

#endif //CAMERA_H