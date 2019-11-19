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
glm::mat4 rotateFromAToB(glm::vec4 A, glm::vec4 B) {
	glm::vec3 crossAB = cross(glm::vec3(A), glm::vec3(B));
	glm::vec4 a = glm::vec4(crossAB / glm::length(crossAB), 0);
	float alpha = glm::acos(glm::dot(glm::vec3(A), glm::vec3(B)));
	float s = sin(alpha);
	float c = cos(alpha);
	return glm::mat4(
		glm::vec4(a.x * a.x * (1 - c) + c, a.x * a.y * (1 - c) - s * a.z, a.x * a.z * (1 - c) - s * a.y, 0),
		glm::vec4(a.x * a.y * (1 - c) - s * a.z, a.y * a.y * (1 - c) + c, a.y * a.z * (1 - c) + s * a.x, 0),
		glm::vec4(a.x * a.z * (1 - c) + s * a.y, a.y * a.z * (1 - c) - s * a.x, a.z * a.z * (1 - c) + c, 0),
		glm::vec4(0, 0, 0, 1)
	);
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

	colorMap = std::make_unique<Texture2D>("./_asset/ball2.png");
	normalMap = std::make_unique<Texture2D>("./_asset/ball_normalmap.png");
	colorMap->init(false);
	normalMap->init(true);

	shaderProgram.use();
	shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
	shaderProgram.setUniformVariable("colorTexture", 0);
	shaderProgram.setUniformVariable("normalTexture", 1);
	//shaderProgram.setUniformVariable("lightDir", glm::normalize(glm::vec4(1, 1, 1, 0)));

	glm::mat4 m = rotateFromAToB(glm::vec4{ 0,0,1,0 }, glm::vec4{ -1,0,0,0 });

}


void FluidObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	if (ParticleData::m_ParticleBufferOpen) {
		LOG_F(WARNING, "Particle Buffer was opened during draw call, aborting");
		return;
	}

	colorMap->useAs(GL_TEXTURE0);
	normalMap->useAs(GL_TEXTURE1);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);
	shaderProgram.setUniformVariable("particleSize", particleSize);
	glm::vec4 ld = glm::vec4(1, 0, 1, 0);
	shaderProgram.setUniformVariable("lightDir", ld);
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

