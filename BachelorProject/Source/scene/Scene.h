#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "./camera/Camera.h"
#include "../materialObjects/MaterialObject.h"
#include <algorithm>

namespace Scene {
	class Scene
	{
	public:
		Scene(glm::vec4 backgroundColor);
		void addCamera(Camera* camera) { cameras.push_back(camera); }
		void addMaterialObject(MaterialObject* materialObject);
		void renderScene();

		std::vector<Camera*> cameras;
	protected:
	private:
		glm::vec4 backgroundColor;

		std::vector<MaterialObject*> objects;
	};
}

