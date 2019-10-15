#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "./camera/Camera.h"
#include <scene/camera/CameraController.h>
#include "../materialObjects/MaterialObject.h"
#include <algorithm>

namespace Scene {
	class Scene
	{
	public:
		Scene(glm::vec4 backgroundColor);
		void addCameras(const CameraController* controller);
		void addMaterialObject(MaterialObject* materialObject);
		void renderScene();
		glm::vec4 getBackgroundColor() { return backgroundColor; }

		std::vector<const Camera*> cameras;
	protected:
	private:
		glm::vec4 backgroundColor;

		std::vector<MaterialObject*> objects;
	};
}

