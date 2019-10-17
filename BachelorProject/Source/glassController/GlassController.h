#pragma once

#include <window/Window.h>
#include <window/listener/KeyPressListener.h>
#include <scene/camera/Camera.h>
#include <vector>
#include <materialObjects/GlassObject.h>
#include <particleObjects/ParticleObjectManager.h>
#include <scene/Scene.h>

class GlassController
	: public KeyPressListener
{
public:
	GlassController(Window& window, const Scene::Camera& camera, const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram, const glm::vec4& background);
	void assignUntrackedObjects(Scene::Scene& scene);
	void handleKeyPress(Key key) override;
	
private:
	int trackedObjects = 0;
	std::vector<GlassObject*> glassObjects;
	const Scene::Camera& camera;
	int currentlySelected = -1;

	const ShaderProgram& shaderProgram;
	const ShaderProgram& selectedProgram;
	const glm::vec4& background;
};

