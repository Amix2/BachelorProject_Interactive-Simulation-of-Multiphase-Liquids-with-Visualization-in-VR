#include "Scene.h"

void Scene::Scene::renderScene()
{
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const Camera* camera : cameras) {
		camera->select();
		for (const MaterialObject* object : objects) {
			object->load(camera->getViewMatrix(), projectionMatrix);
		}
	}
}