#pragma once
#include "MaterialObject.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <dataStructures/GpuResources.h>
#include <Configuration.h>
#include <dataStructures/ParticleData.h>

class FluidObject :
	public MaterialObject
{
public:
	FluidObject(const ShaderProgram& shaderProgram);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
private:

	unsigned int VAO;
	unsigned int textureID;
};

