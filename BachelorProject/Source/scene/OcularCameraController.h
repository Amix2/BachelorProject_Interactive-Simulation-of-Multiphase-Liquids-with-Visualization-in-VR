#pragma once

#include "MousePositionListener.h"
#include "MouseScrollListener.h"
#include "WindowSizeListener.h"
#include "Window.h"
#include "Camera.h"
#include <glm/glm.hpp>

class OcularCameraController
	: public MousePositionListener
	, public MouseScrollListener
{
public:
	OcularCameraController(Window& window, ViewPort& leftEyeViewPort, ViewPort& rightEyeViewPort, float eyesDistance, const glm::vec3& position);
	
	void handleMouseMove(double xoffset, double yoffset) override;
	void handleMouseScroll(double scroll) override;
	Scene::Camera& getRightCamera() { return rightEye; }
	Scene::Camera& getLeftCamera() { return leftEye; }
private:
	inline static const double MOUSE_SENSITIVITY = 0.1;
	inline static const glm::vec3 WORLD_UP{ 0.0f, 1.0f, 0.0f };
	inline static const float DEFAULT_CAM_PITCH{ 0.0f };
	inline static const float DEFAULT_CAM_YAW{ 0.0f };
	inline static const float DEFAULT_CAM_ROLL{ 0.0f };

	float eyesDistance;
	glm::vec3 headUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	float pitch;
	float yaw;
	float roll;



	Scene::Camera leftEye;
	Scene::Camera rightEye;

	void updateHeadVectors();
};