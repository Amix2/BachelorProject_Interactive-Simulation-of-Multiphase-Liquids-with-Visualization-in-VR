#pragma once

#include <vector>
#include "Camera.h"
#include <simulationObjects/EmiterProvider.h>

class CameraController
	: public EmiterProvider 
{
public:
	virtual const std::vector<const Scene::Camera*> provideCameras() const = 0;
};