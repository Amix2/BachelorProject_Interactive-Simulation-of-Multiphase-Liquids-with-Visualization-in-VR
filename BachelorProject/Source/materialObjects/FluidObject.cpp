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

	int index = GpuResources::getIndex(BufferDetails::particlePositionsName);
	glBindBuffer(GL_ARRAY_BUFFER, index);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	int width;
	int height;
	int nrChannels;
	unsigned char* data = stbi_load("./_asset/ball.png", &width, &height, &nrChannels, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float textureBorderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //transparent
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
		//return;
	}
	int index = GpuResources::getIndex(BufferDetails::particlePositionsName);
	glBindBuffer(GL_ARRAY_BUFFER, index);
	glBindTexture(GL_TEXTURE_2D, textureID);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);
	shaderProgram.setUniformVariable("particleSize", particleSize);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0,ParticleData::m_NumOfParticles);
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

