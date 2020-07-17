#include "AxesObject.h"

AxesObject::AxesObject(ShaderProgram shaderProgram)
	: MaterialObject{ shaderProgram } {}


void AxesObject::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shaderProgram.use();
	shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
}

void AxesObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	if (ParticleData::m_ParticleBufferOpen) {
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);

	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, 6);
}
