#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Camera.h"
#include "MaterialObject.h"
#include <algorithm>

namespace Scene {
	class Scene
	{
	public:
		void addCamera(const Camera& camera) { cameras.push_back(camera); }
		bool addMaterialObject(const MaterialObject& materialObject) { objects.push_back(materialObject); }
		void renderScene();

	protected:
	private:
		std::vector<double> backgroundColor;
		glm::mat4 projectionMatrix = glm::perspective(1.0f, 600.0f / 800.0f, 0.1f, 100.0f);

		std::vector<Camera> cameras;
		std::vector<MaterialObject> objects
	};
}

