#include "Scene.h"

Scene::Scene::Scene(glm::vec4 backgroundColor, int numberOfLayers)
	: backgroundColor{ backgroundColor } 
	, numberOfLayers{ numberOfLayers }
	, layers( numberOfLayers, std::vector<MaterialObject*>()) 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0x00);
	Scene::currentScene = this;
}

void Scene::Scene::addCameras(const CameraController* controller)
{
	for (const Camera* camera : controller->provideCameras()) {
		cameras.push_back(camera);
	}
}

void Scene::Scene::addMaterialObject(MaterialObject* materialObject, unsigned int layer)
{
	layers[layer].push_back(materialObject);
	materialObject->init();

}

void Scene::Scene::renderScene()
{
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (const Camera* camera : cameras) {
		camera->select();
		for (const auto objects : layers) {
			for (const MaterialObject* object : objects) {
				object->load(camera->getViewMatrix(), camera->getProjectionMatrix());
			}
		}
	}

}