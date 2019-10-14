#include "TestMaterialObject.h"
#include <Logger.h>

TestMaterialObject::TestMaterialObject(const ShaderProgram& shaderProgram, glm::vec4 background) 
	: MaterialObject{ shaderProgram }
	, background{ background } {}

void TestMaterialObject::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void TestMaterialObject::load(const glm::mat4& view, const glm::mat4& projection) const
{ 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shaderProgram.use();
	shaderProgram.setUniformVariable("projection", projection);
	shaderProgram.setUniformVariable("view", view);
	shaderProgram.setUniformVariable("background", background);

	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i + 1.0f;
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shaderProgram.setUniformVariable("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}
