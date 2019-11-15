#pragma once

#include <scene/Scene.h>
#include <inputDispatcher/InputDispatcher.h>
#include <vector>
#include <materialObjects/GlassObject.h>
#include <particleObjects/ParticleObjectManager.h>

class GlassController
	: public KeyInputListener
	, public MousePositionListener
{
public:
	GlassController(InputDispatcher& inputDispatcher, const Scene::Camera& camera, const ShaderProgram& shaderProgram, const ShaderProgram& selectedProgram);
	void assignUntrackedObjects(Scene::Scene& scene);
	void handleKeyPress(int key, KeyState action, float deltaTime) override;
	void handleMouseMove(float x, float y) override;
	int getMode() const { return mode; };
	bool isDestinationValid() const { return destinationValid; };
	float getMoveAccumulator() const { return moveAccumulator; };
	
	const glm::vec3& getSelectedAxis() const { return selectedAxis; };
	const glm::mat4* getCurrentlySelectedModelMatrix() const;
	const glm::mat4* getCurrentlySelectedDestinationMatrix() const;
	
private:
	const inline static float VELOCITY = 20.0f;
	const inline static float SENSITIVITY = 0.1f;

	int trackedObjects = 0;
	std::vector<GlassObject*> glassObjects;
	const Scene::Camera& camera;
	int currentlySelected = -1;

	glm::vec3 selectedAxis{ 1.0f, 0.0, 0.0 };
	float moveAccumulator{ 0.0f };
	enum Mode{
		MOVE,
		ROTATE
	} mode{ MOVE };
	bool destinationValid = true;

	const ShaderProgram& shaderProgram;
	const ShaderProgram& selectedProgram;

	void selectGlass();
	void submitMove();
};

