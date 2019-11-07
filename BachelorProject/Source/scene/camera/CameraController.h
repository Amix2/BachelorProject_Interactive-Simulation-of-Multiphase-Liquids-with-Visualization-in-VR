#pragma once

#include <vector>
#include "Camera.h"
#include <simulationObjects/EmiterMatrixProvider.h>

class CameraController
	: public EmiterMatrixProvider 
{
public:
	virtual const std::vector<const Scene::Camera*> provideCameras() const = 0;
};