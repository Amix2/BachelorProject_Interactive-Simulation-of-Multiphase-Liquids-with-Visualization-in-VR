#pragma once
#include <materialObjects/MaterialObject.h>
#include <Configuration.h>
#include <dataStructures/ParticleData.h>
#include <Logger.h>

class AxesObject : public MaterialObject {
public:
	AxesObject(ShaderProgram shaderProgram);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
private:
	unsigned int VBO;
	unsigned int VAO;

	glm::vec4 background;

	float vertices[36]{
		1000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-1000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 1000.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -1000.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1000.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1000.0f, 0.0f, 0.0f, 1.0f,
	};
};