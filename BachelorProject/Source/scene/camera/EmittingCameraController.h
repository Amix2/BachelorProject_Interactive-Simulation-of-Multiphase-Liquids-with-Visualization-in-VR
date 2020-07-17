#pragma once
#include <inputDispatcher/InputDispatcher.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "SimpleCameraController.h"
#include <emiters/EmiterManager.h>



class EmittingCameraController
	: public MousePositionListener
	, public MouseScrollListener
	, public SimpleCameraController
	, public KeyInputListener
{
public:
	EmittingCameraController(InputDispatcher& inputDispatcher, ViewPort & viewPort, const glm::vec3 & position);

	void handleMouseMove(float x, float y) override;
	void handleMouseScroll(float scroll) override;
	void handleKeyPress(int key, KeyState action, float deltaTime) override;

	const std::vector<const Scene::Camera*> provideCameras() const override;

	Emiter* emiter;
	glm::mat4 getEmiterMatrix() const;

private:
	bool m_updateEmiterMatrix = true;

};