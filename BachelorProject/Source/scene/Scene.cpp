#include "Scene.h"

void Scene::Scene::renderScene()
{
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::for_each(cameras.begin(), cameras.end(), [&](const Camera& camera) {

		std::for_each(objects.begin(), objects.end(), [&](const MaterialObject& object)
			{
				object.load(camera.getViewMatrix(), projectionMatrix);
			});
		});
}
