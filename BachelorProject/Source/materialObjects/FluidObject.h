#pragma once
#include "MaterialObject.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <dataStructures/GpuResources.h>
#include <Configuration.h>
#include <dataStructures/ParticleData.h>
#include <inputDispatcher/InputDispatcher.h>
#include <Configuration.h>

class FluidObject :
	public MaterialObject,
	public KeyInputListener
{
public:
	FluidObject(InputDispatcher& inputDispatcher, const ShaderProgram& shaderProgram);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
	void handleKeyPress(int key, KeyState action, float deltaTime)  override;
private:
	inline static const float INITIAL_PARTICLES_SIZE = 0.3;

	unsigned int VAO;
	unsigned int instanceVBO;
	unsigned int textureID;
	float particleSize;

	unsigned int quadVBO;
	float quadVertices[4 * 4]{
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
};

