#pragma once

#include <vector>
#include "Camera.h"
#include <emiters/EmiterProvider.h>

class CameraController
{
public:
	virtual const std::vector<const Scene::Camera*> provideCameras() const = 0;
};