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
	OcularCameraController(Window& window, Scene::Camera& leftEye, Scene::Camera& rightEye);
	
	void handleMouseMove(double xoffset, double yoffset) override;
	void handleMouseScroll(double scroll) override;
private:
	inline static const double MOUSE_SENSITIVITY = 0.1;

	Scene::Camera* leftEye;
	Scene::Camera* rightEye;
};