#pragma once

#include <scene/Scene.h>
#include <inputDispatcher/InputDispatcher.h>
#include <vector>
#include <materialObjects/GlassObject.h>
#include <particleObjects/ParticleObjectManager.h>

class GlassController
	: public KeyInputListener
{
public:
	GlassController(InputDispatcher& inputDispatcher, const Scene::Camera& camera, const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram);
	void assignUntrackedObjects(Scene::Scene& scene);
	void handleKeyPress(int key, KeyState action, float deltaTime) override;
	
private:
	const inline static float VELOCITY = 20.0f;

	int trackedObjects = 0;
	std::vector<GlassObject*> glassObjects;
	const Scene::Camera& camera;
	int currentlySelected = -1;

	const ShaderProgram& shaderProgram;
	const ShaderProgram& selectedProgram;

	void selectGlass();
};

