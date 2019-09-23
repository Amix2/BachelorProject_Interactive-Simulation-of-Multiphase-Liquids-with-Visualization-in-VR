#include "TestBilboardObject.h"

TestBilboardObject::TestBilboardObject(const ShaderProgram& shaderProgram)
	: MaterialObject{ shaderProgram } {}

void TestBilboardObject::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int width;
	int height;
	int nrChannels;
	unsigned char* data = stbi_load("./Source/scene/ball.png", &width, &height, &nrChannels, 0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float textureBorderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //transparent
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

}

void TestBilboardObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindTexture(GL_TEXTURE_2D, textureID);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);

	glBindVertexArray(VAO);

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) 
			for (int k = 0; k < 5; k++) {
				glm::mat4 model = glm::mat4{ 1.0f };
				model = glm::scale(model, glm::vec3{ 2, 2, 2 });
				model = glm::translate(model, glm::vec3{ 4 + k, i, j});
				shaderProgram.setUniformVariable("model", model);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}



}
