#include "PyramidPointerMaterialObject.h"
#include <VR/VRGeometry.h>


PyramidPointerMaterialObject::PyramidPointerMaterialObject(ShaderProgram shaderProgram, glm::vec4 PyramidColor, const MaterialObjectOwner* owner)
	: MaterialObject{ shaderProgram }, PyramidColor{ PyramidColor }, m_owner{ owner }
{
}

bool PyramidPointerMaterialObject::InitializeBufferObjects() {

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), this->Vertices, GL_DYNAMIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLuint), this->Elements, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return true;
}

void PyramidPointerMaterialObject::init() {
	//Emiter* e = &(EmiterManager::m_emitersVector[0]);
	this->InitializeBufferObjects();
	//this->InitializeVertices();
	this->shaderProgram.use();
	checkOpenGLErrors();

	this->shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
}

void PyramidPointerMaterialObject::load(const glm::mat4& view, const glm::mat4& projection) const {
	auto paramsMap = m_owner->getAdditionalParameters();

	const glm::vec4* color = &PyramidColor;
	if (paramsMap.count(Params::COLOR)) {
		color = &(paramsMap[Params::COLOR].vec4Value);
	}

	glBindVertexArray(this->VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	this->shaderProgram.use();
	//LOG_F(WARNING, "~~\n%s\n%s", glm::to_string(m_owner->getModel()).c_str(), glm::to_string(projection * view * m_owner->getModel()).c_str());
	this->shaderProgram.setUniformVariable("MVP", projection * view * m_owner->getModel());
	this->shaderProgram.setUniformVariable("pyramidColor", *color);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

}
