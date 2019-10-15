#include "Scene.h"

Scene::Scene::Scene(glm::vec4 backgroundColor)
	: backgroundColor{ backgroundColor } {}

void Scene::Scene::addCameras(const CameraController* controller)
{
	for (const Camera* camera : controller->provideCameras()) {
		cameras.push_back(camera);
	}
}

void Scene::Scene::addMaterialObject(MaterialObject* materialObject)
{
	objects.push_back(materialObject);
	materialObject->init();
}

void Scene::Scene::renderScene()
{
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const Camera* camera : cameras) {
		camera->select();
		for (const MaterialObject* object : objects) {
			object->load(camera->getViewMatrix(), camera->getProjectionMatrix());
		}
	}
}