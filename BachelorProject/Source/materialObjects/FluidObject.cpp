#include "FluidObject.h"
#include <Logger.h>

FluidObject::FluidObject(InputDispatcher& window, const ShaderProgram& shaderProgram)
	: MaterialObject{ shaderProgram } 
	, particleSize{ INITIAL_PARTICLES_SIZE }
{
	window.subscribeForKeyInput(this, std::vector<int>{
		GLFW_KEY_1,
		GLFW_KEY_2
	});
}

void FluidObject::init()
{

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	instanceVBO = GpuResources::getIndex(BufferDetails::particlePositionsName);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	int width;
	int height;
	int nrChannels;
	unsigned char* data = stbi_load("./_asset/ball2.png", &width, &height, &nrChannels, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float textureBorderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //transparent
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG_F(ERROR, "Failed to load texture");
	} 
	stbi_image_free(data);

	shaderProgram.use();
	shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);

}

void FluidObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	if (ParticleData::m_ParticleBufferOpen) {
		LOG_F(WARNING, "Particle Buffer was opened during draw call, aborting");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, textureID);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);
	shaderProgram.setUniformVariable("particleSize", particleSize);

	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticleData::m_NumOfParticles);
}

void FluidObject::handleKeyPress(int key, KeyState action, float deltaTime)
{
	if (action == KeyState::FALLING_EDGE) {
		float deltaSize{ 0.0f };
		switch (key) {
		case GLFW_KEY_1:
			deltaSize = -0.05f;
			break;
		case GLFW_KEY_2:
			deltaSize = 0.05f;
			break;
		default:
			break;
		}
		if (deltaSize != 0.0f) {
			particleSize += deltaSize;
			if (particleSize < 0.0f)
				particleSize = 0.0f;
		}
	}
}

