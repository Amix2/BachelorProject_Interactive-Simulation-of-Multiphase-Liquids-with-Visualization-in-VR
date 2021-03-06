#pragma once
#include <inputDispatcher/InputDispatcher.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "CameraController.h"
#include <emiters/EmiterManager.h>


class SimpleCameraController
	: public MousePositionListener
	, public MouseScrollListener
	, public CameraController
	, public KeyInputListener
{
public:
	SimpleCameraController(InputDispatcher& inputDispatcher, ViewPort & viewPort, const glm::vec3 & position);

	void handleMouseMove(float x, float y) override;
	void handleMouseScroll(float scroll) override;
	void handleKeyPress(int key, KeyState action, float deltaTime) override;

	const std::vector<const Scene::Camera*> provideCameras() const override;


protected:
	inline static const double MOUSE_SENSITIVITY = 0.1;
	inline static const float VELOCITY = 100.0f;

	bool isLocked = false;

	Scene::Camera camera;
};