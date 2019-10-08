#pragma once
#include "../../window/listener/MousePositionListener.h"
#include "../../window/listener/MouseScrollListener.h"
#include "../../window/listener/KeyPressListener.h"
#include "../../window/Window.h"
#include <glm/glm.hpp>
#include "Camera.h"

class SimpleCameraController
	: public MousePositionListener
	, public MouseScrollListener 
	, public KeyPressListener
{
public:
	SimpleCameraController(Window & window, ViewPort & viewPort, const glm::vec3 & position);
	~SimpleCameraController();

	void handleMouseMove(double xoffset, double yoffset) override;
	void handleMouseScroll(double scroll) override;
	void handleKeyPress(Key key) override;

	Scene::Camera& getCamera() { return camera; }

	inline static Scene::Camera* cam;
private:
	inline static const double MOUSE_SENSITIVITY = 0.1;
	inline static const float VELOCITY = 10.0f;

	Window* window;

	Scene::Camera camera;
};