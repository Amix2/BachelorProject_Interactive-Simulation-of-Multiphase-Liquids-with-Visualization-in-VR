#pragma once

#include "MaterialObject.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>

class TestBilboardObject : public MaterialObject
{
public:
	TestBilboardObject(const ShaderProgram& shaderProgram);
	void init() override;
	void load(const glm::mat4& view, const glm::mat4& projection) const override;
private:

	//vertices
	unsigned int VBO;
	unsigned int VAO;

	float vertices[12] = {
		 10.0f, 0.0f, 0.0f,

	};


	//texture
	unsigned int textureID;
};

