#include "Scene.h"

Scene::Scene::Scene(glm::vec4 backgroundColor)
	: backgroundColor{ backgroundColor } {}

void Scene::Scene::addMaterialObject(MaterialObject* materialObject)
{
	objects.push_back(materialObject);
	materialObject->init();
}

void Scene::Scene::renderScene()
{
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0x00);

	for (const Camera* camera : cameras) {
		camera->select();
		for (const MaterialObject* object : objects) {
			object->load(camera->getViewMatrix(), camera->getProjectionMatrix());
		}
	}
}