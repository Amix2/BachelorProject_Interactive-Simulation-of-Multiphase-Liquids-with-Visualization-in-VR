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
		void addCamera(Camera* camera) { cameras.push_back(camera); }
		void addMaterialObject(MaterialObject* materialObject);
		void renderScene();

		std::vector<Camera*> cameras;
	protected:
	private:
		std::vector<GLfloat> backgroundColor{ 0.1f, 0.1f, 0.4f, 1.0f};
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 600.0f / 600.0f, 0.1f, 200.0f);


		std::vector<MaterialObject*> objects;
	};
}

