#pragma once

#include <map>

#include "MaterialObject.h"
#include <Configuration.h>
#include <vector>

class PyramidPointerMaterialObject final : public MaterialObject {
public:
	PyramidPointerMaterialObject(ShaderProgram ShaderProgram, glm::vec4 PyramidColor);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;

protected:
	//

private:
	bool InitializeBufferObjects();
	//bool InitializeVertices();
	std::map<std::string, GLuint> BufferObjects{};
	GLuint VAO, VBO, EBO;
	glm::vec4 PyramidColor{};
	const unsigned long BaseVertices = 3;
	const float TETRAHEDRON_RADIUS = 0.6124f;
	const float TETRAHEDRON_HEIGHT = 0.8165f;
	const float TETRAHEDRON_BASE_HEIGHT = 0.866f;
	double PyramidHeight;
	double PyramidRadius;

	GLfloat Vertices[3 * (1 + 3)]{ 0.0f, TETRAHEDRON_RADIUS, 0.0f,
								0.0f, TETRAHEDRON_RADIUS - TETRAHEDRON_HEIGHT, 2 * TETRAHEDRON_BASE_HEIGHT / 3,
								0.5f, TETRAHEDRON_RADIUS - TETRAHEDRON_HEIGHT, -(TETRAHEDRON_BASE_HEIGHT / 3),
								-0.5f, TETRAHEDRON_RADIUS - TETRAHEDRON_HEIGHT, -(TETRAHEDRON_BASE_HEIGHT / 3)};

	GLuint Elements[4 * 3]{
		1, 2, 3,
		1, 2, 0,
		3, 1, 0,
		2, 3, 0
	};
/*
	glm::vec3 CalculateBaseVertexPosition(double PyramidRadius, double PyramidHeight);
	glm::vec3 CalculateBaseVertexCoordinateForIndex(unsigned int Index, double PyramidRadius, double PyramidHeight) const;*/

};
