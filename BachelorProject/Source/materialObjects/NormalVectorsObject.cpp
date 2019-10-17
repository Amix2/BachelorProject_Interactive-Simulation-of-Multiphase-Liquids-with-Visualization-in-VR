#include "NormalVectorsObject.h"

NormalVectorsObject::NormalVectorsObject(Window& window, ShaderProgram program, glm::vec4 background)
	: MaterialObject{ program }
	, background{ background } 
{
	window.subscribeForKeyInput(this);
}

void NormalVectorsObject::init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	VBO = GpuResources::getIndex(BufferDetails::glassParticleName);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void NormalVectorsObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);
	shaderProgram.setUniformVariable("background", background);
	shaderProgram.setUniformVariable("render", render);


	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, ParticleData::m_NumOfGlassParticles);
}


void NormalVectorsObject::handleKeyPress(Key key)
{
	switch (key) {
	case KEY_9:
		render = !render;
		break;
	default:
		break;
	}
}
