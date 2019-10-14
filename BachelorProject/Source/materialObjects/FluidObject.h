#pragma once
#include "MaterialObject.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <dataStructures/GpuResources.h>
#include <Configuration.h>
#include <dataStructures/ParticleData.h>
#include <window/listener/KeyPressListener.h>
#include <window/Window.h>

class FluidObject :
	public MaterialObject,
	public KeyPressListener
{
public:
	FluidObject(Window& window, const ShaderProgram& shaderProgram, const glm::vec4& background);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
	void handleKeyPress(Key key) override;
private:
	inline static const float INITIAL_PARTICLES_SIZE = 0.3;

	unsigned int VAO;
	unsigned int textureID;
	float particleSize;
	bool renderGlass = true;
	glm::vec4 background;
};

