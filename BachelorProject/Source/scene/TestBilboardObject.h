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
	unsigned int EBO;
	unsigned int VAO;

	float vertices[32] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[6] = {
	 0, 1, 3, // first triangle
	 1, 2, 3  // second triangle
	};


	//texture
	unsigned int textureID;
};

