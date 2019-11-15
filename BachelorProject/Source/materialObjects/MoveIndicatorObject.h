#pragma once

#include "MaterialObject.h"
#include <glassController/GlassController.h>
#include <inputDispatcher/InputDispatcher.h>
#include <glm/gtx/vector_angle.hpp>

class MoveIndicatorObject final :
	public MaterialObject,
	public KeyInputListener
{
public:
	MoveIndicatorObject(InputDispatcher& inputDispatcher, const ShaderProgram& shaderProgram, GlassController* glassController);

	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
	void handleKeyPress(int key, KeyState state, float deltaTime) override;
private:
	GlassController* glassController;
	bool renderHelper;

	const float TETRAHEDRON_RADIUS = 0.6124f;
	const float TETRAHEDRON_HEIGHT = 0.8165f;
	const float TETRAHEDRON_BASE_HEIGHT = 0.866f;

	GLuint indicatorVBO;
	GLuint indicatorVAO;
	GLuint indicatorEBO;
	GLfloat indicatorVertices[12]{
		0.0f, TETRAHEDRON_RADIUS, 0.0f,
		0.0f, TETRAHEDRON_RADIUS - TETRAHEDRON_HEIGHT, 2 * TETRAHEDRON_BASE_HEIGHT / 3,
		0.5f, TETRAHEDRON_RADIUS - TETRAHEDRON_HEIGHT, -(TETRAHEDRON_BASE_HEIGHT / 3),
		-0.5f, TETRAHEDRON_RADIUS - TETRAHEDRON_HEIGHT, -(TETRAHEDRON_BASE_HEIGHT / 3)
	};
	GLuint indicatorIndices[12]{
		1, 2, 3,
		1, 2, 0,
		3, 1, 0,
		2, 3, 0
	};

	GLuint helperVBO;
	GLuint helperVAO;
	GLfloat helperVertices[6]{
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	GLuint rhelperVBO;
	GLuint rhelperVAO;
	GLfloat rhelperVertices[3 * 21]{

	};
};