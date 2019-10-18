#pragma once
#include <inputDispatcher/InputDispatcher.h>
#include <glm/glm.hpp>
#include "Camera.h"

class SimpleCameraController
	: public MousePositionListener
	, public MouseScrollListener 
	, public KeyInputListener
{
public:
	SimpleCameraController(InputDispatcher& inputDispatcher, ViewPort & viewPort, const glm::vec3 & position);

	void handleMouseMove(float x, float y) override;
	void handleMouseScroll(float scroll) override;
	void handleKeyPress(int key, KeyState action, float deltaTime) override;

	Scene::Camera& getCamera() { return camera; }

private:
	inline static const double MOUSE_SENSITIVITY = 0.1;
	inline static const float VELOCITY = 20.0f;

	Scene::Camera camera;
};