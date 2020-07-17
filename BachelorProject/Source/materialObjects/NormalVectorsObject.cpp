#include "NormalVectorsObject.h"

NormalVectorsObject::NormalVectorsObject(InputDispatcher& inputDispatcher, ShaderProgram program)
	: MaterialObject{ program }
{
	inputDispatcher.subscribeForKeyInput(this, GLFW_KEY_9);
}

void NormalVectorsObject::init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	VBO = GpuResources::getIndex(BufferDetails::glassParticleName);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void NormalVectorsObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);
	shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
	shaderProgram.setUniformVariable("render", render);


	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, ParticleData::m_NumOfGlassParticles);
}


void NormalVectorsObject::handleKeyPress(int key, KeyState action, float deltaTime)
{
	if (action == KeyState::FALLING_EDGE) {
		switch (key) {
		case GLFW_KEY_9:
			render = !render;
			break;
		default:
			break;
		}
	}
}
