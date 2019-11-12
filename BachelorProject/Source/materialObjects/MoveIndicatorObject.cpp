#include "MoveIndicatorObject.h"

MoveIndicatorObject::MoveIndicatorObject(InputDispatcher& inputDispatcher, const ShaderProgram& shaderProgram, GlassController* glassController)
	: MaterialObject{ shaderProgram }
	, glassController{ glassController } 
{
	inputDispatcher.subscribeForKeyInput(this, GLFW_MOUSE_BUTTON_LEFT);
}

void MoveIndicatorObject::init()
{
	glGenVertexArrays(1, &indicatorVAO);
	glBindVertexArray(indicatorVAO);

	glGenBuffers(1, &indicatorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, indicatorVBO);

	glGenBuffers(1, &indicatorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicatorEBO);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), this->indicatorVertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLuint), this->indicatorIndices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &helperVAO);
	glBindVertexArray(helperVAO);

	glGenBuffers(1, &helperVBO);
	glBindBuffer(GL_ARRAY_BUFFER, helperVBO);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), this->helperVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &rhelperVAO);
	glBindVertexArray(rhelperVAO);

	glGenBuffers(1, &rhelperVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rhelperVBO);

	glm::vec4 cursor{ 0.5, 0, 0, 1 };
	for (int i = 0; i < 20; i++) {
		rhelperVertices[i * 3] = cursor.x;
		rhelperVertices[i * 3 + 1] = cursor.y;
		rhelperVertices[i * 3 + 2] = cursor.z;
		cursor = glm::rotateZ(cursor, glm::pi<float>() / 9.5f);
	}
	rhelperVertices[60] = rhelperVertices[61] = rhelperVertices[62] = 0;

	glBufferData(GL_ARRAY_BUFFER, 60 * sizeof(GLfloat), this->rhelperVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


}

void MoveIndicatorObject::load(const glm::mat4& view, const glm::mat4& projection) const
{
	const glm::mat4* model = glassController->getCurrentlySelectedModelMatrix();
	const glm::mat4* destination = glassController->getCurrentlySelectedDestinationMatrix();

	if (model == nullptr || destination == nullptr)
		return;
	glm::vec3 diff{ (*destination)[3][0] - (*model)[3][0], (*destination)[3][1] - (*model)[3][1], (*destination)[3][2] - (*model)[3][2] };

	shaderProgram.use();
	if ((glm::abs(glassController->getMoveAccumulator()) > 1.0f && !glassController->isDestinationValid() && glassController->getMode() == 0) || glm::length(diff) > 1.0f) { //RENDER INDICATOR
		glBindVertexArray(indicatorVAO);
		glBindBuffer(GL_ARRAY_BUFFER, indicatorVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicatorEBO);
		shaderProgram.setUniformVariable("color", glm::vec4{ 1, 1, 0.0f, 1.0f });
		if (!glassController->isDestinationValid() && glassController->getMode() == 0) {
			shaderProgram.setUniformVariable("MVP", projection * view * *destination * glm::translate(glm::mat4{ 1.0f }, glassController->getSelectedAxis() * glassController->getMoveAccumulator()) * glm::scale(glm::mat4{ 1 }, glm::vec3{ 5, 5, 5 }));
		}
		else {
			shaderProgram.setUniformVariable("MVP", projection * view * *destination * glm::scale(glm::mat4{ 1 }, glm::vec3{ 5, 5, 5 }));
		}
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}
	if (renderHelper && glassController->getMode() == 0) {
		glBindVertexArray(helperVAO);
		glBindBuffer(GL_ARRAY_BUFFER, helperVBO);
		glm::mat4 mod = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 90, 1, 1 });
		if (glassController->getSelectedAxis().y > 0.0f) {
			mod = glm::rotate(glm::mat4{ 1.0f }, glm::pi<float>() * 0.5f, glm::vec3{ 0.0f, 0.0f, 1.0f }) * mod;
			shaderProgram.setUniformVariable("color", { 0, 1, 0, 1 });
		}
		else if (glassController->getSelectedAxis().z > 0.0f) {
			mod = glm::rotate(glm::mat4{ 1.0f }, glm::pi<float>() * 0.5f, glm::vec3{ 0.0f, 1.0f, 0.0f }) * mod;
			shaderProgram.setUniformVariable("color", { 0, 0, 1, 1 });
		}
		else {
			shaderProgram.setUniformVariable("color", { 1, 0, 0, 1 });
		}


		shaderProgram.setUniformVariable("MVP", projection * view * *destination * mod);
		glDrawArrays(GL_LINES, 0, 2);
	}

	//float cosX = glm::angle(glm::vec3{ (*model)[0][0], (*model)[0][1], (*model)[0][2] }, glm::vec3{ (*destination)[0][0], (*destination)[0][1], (*destination)[0][2] });
	//float cosY = glm::angle(glm::vec3{ (*model)[1][0], (*model)[1][1], (*model)[1][2] }, glm::vec3{ (*destination)[1][0], (*destination)[1][1], (*destination)[1][2] });
	//float cosZ = glm::angle(glm::vec3{ (*model)[2][0], (*model)[2][1], (*model)[2][2] }, glm::vec3{ (*destination)[2][0], (*destination)[2][1], (*destination)[2][2] });

	//if ((glm::abs(glassController->getMoveAccumulator()) > 1.0f && !glassController->isDestinationValid() && glassController->getMode() == 1) || cosX > 0.1f || cosY > 0.1f ) { //RENDER INDICATOR
	//	glBindVertexArray(indicatorVAO);
	//	glBindBuffer(GL_ARRAY_BUFFER, indicatorVBO);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicatorEBO);
	//	shaderProgram.setUniformVariable("color", glm::vec4{ 0, 1, 1, 1.0f });
	//	glm::mat4 translate = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ 0.0f, 45.0f, 0.0f });
	//	if (!glassController->isDestinationValid() && glassController->getMode() == 0) {

	//		shaderProgram.setUniformVariable("MVP", projection * view * *destination * glm::translate(glm::mat4{ 1.0f }, glassController->getSelectedAxis() * glassController->getMoveAccumulator()) );
	//	}
	//	else {
	//		glm::mat4 rotation = glm::rotate(glm::mat4{ 1.0f }, glm::acos(cosX), glm::acos(cosX), glm::acos(cosY), glm::acos(cosZ) });
	//		shaderProgram.setUniformVariable("MVP", projection * view * *destination * glm::scale(glm::mat4{ 1 }, glm::vec3{ 5, 5, 5 }));
	//	}
	//	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	//}
	if (renderHelper && glassController->getMode() == 1) {
		glBindVertexArray(rhelperVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rhelperVBO);
		glm::mat4 mod = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 90, 90, 1 });
		float initialRotation = glm::pi<float>() * 0.5f;
		if (!glassController->isDestinationValid())
			initialRotation += glassController->getMoveAccumulator() * 0.1f;
			mod = glm::rotate(glm::mat4{ 1.0f }, initialRotation, glm::vec3{ 0.0f, 0.0f, 1.0f }) * mod;
		if (glassController->getSelectedAxis().y > 0.0f) {
			mod = glm::rotate(glm::mat4{ 1.0f }, glm::pi<float>() * 0.5f, glm::vec3{ 1.0f, 0.0f, 0.0f }) * mod;
			shaderProgram.setUniformVariable("color", { 0, 1, 0, 1 });
		}
		else if (glassController->getSelectedAxis().z > 0.0f) {
			shaderProgram.setUniformVariable("color", { 0, 0, 1, 1 });
		}
		else {
			mod = glm::rotate(glm::mat4{ 1.0f }, glm::pi<float>() * 0.5f, glm::vec3{ 0.0f, 1.0f, 0.0f }) * mod;
			shaderProgram.setUniformVariable("color", { 1, 0, 0, 1 });
		}
		glm::mat4 rotatedModel{ (*destination)[0][0], (*destination)[0][1], (*destination)[0][2], 0,
			(*destination)[1][0], (*destination)[1][1], (*destination)[1][2], 0,
			(*destination)[2][0], (*destination)[2][1], (*destination)[2][2], 0,
			(*model)[3][0], (*model)[3][1], (*model)[3][2], 1
		};
		shaderProgram.setUniformVariable("MVP", projection * view * rotatedModel * mod);
		glDrawArrays(GL_LINE_STRIP, 0, 21);
	}
}

void MoveIndicatorObject::handleKeyPress(int key, KeyState state, float deltaTime)
{
	if (state == KeyState::FALLING_EDGE && key == GLFW_MOUSE_BUTTON_LEFT)
		renderHelper = true;
	else if (state == KeyState::RISING_EDGE && key == GLFW_MOUSE_BUTTON_LEFT)
		renderHelper = false;
}
