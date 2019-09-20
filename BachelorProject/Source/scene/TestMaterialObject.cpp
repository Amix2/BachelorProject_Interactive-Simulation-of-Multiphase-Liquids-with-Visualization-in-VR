#include "TestMaterialObject.h"


TestMaterialObject::TestMaterialObject(const ShaderProgram& shaderProgram) 
	: MaterialObject{ shaderProgram } {}

void TestMaterialObject::load(const glm::mat4& view, const glm::mat4& projection) const
{ 
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);
	glPointSize(4.0f);
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f); 
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i + 1.0f;
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shaderProgram.setUniformVariable("model", model);

		glDrawArrays(GL_POINTS, 0, 36);
	}

}
