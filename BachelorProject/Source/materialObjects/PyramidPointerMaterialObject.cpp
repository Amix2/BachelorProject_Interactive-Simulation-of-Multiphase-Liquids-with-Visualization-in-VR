#include "PyramidPointerMaterialObject.h"
#include <VR/VRGeometry.h>


PyramidPointerMaterialObject::PyramidPointerMaterialObject(ShaderProgram ShaderProgram, glm::vec4 PyramidColor, const MaterialObjectOwner* owner)
	: MaterialObject{ ShaderProgram }, PyramidColor{ PyramidColor }, m_owner{ owner }
{
}

bool PyramidPointerMaterialObject::InitializeBufferObjects() {
	GLuint VertexArrayObject{};
	GLuint VertexBufferObject{};
	GLuint ElementBufferObject{};

	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);

	glGenBuffers(1, &ElementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), this->Vertices, GL_DYNAMIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLuint), this->Elements, GL_DYNAMIC_DRAW);

	this->VAO = VertexArrayObject;
	this->VBO = VertexBufferObject;
	this->EBO = ElementBufferObject;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return true;
}

void PyramidPointerMaterialObject::init() {
	//Emiter* e = &(EmiterManager::m_emitersVector[0]);
	this->InitializeBufferObjects();
	//this->InitializeVertices();
	this->shaderProgram.use();
	this->shaderProgram.setUniformVariable("background", Configuration::BACKGROUND);
}

void PyramidPointerMaterialObject::load(const glm::mat4& view, const glm::mat4& projection) const {
	auto paramsMap = m_owner->getAdditionalParameters();

	const glm::vec4* color = &PyramidColor;
	if (paramsMap.count(Params::COLOR)) {
		color = &(paramsMap[Params::COLOR].vec4Value);
	}

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	vr::TrackedDevicePose_t trackedDevicePose;
	vr::VRControllerState_t controllerState;
	int deviceId;
	this->shaderProgram.use();
	//LOG_F(WARNING, "%s", glm::to_string(m_owner->getModel()).c_str());
	this->shaderProgram.setUniformVariable("MVP", projection * view * m_owner->getModel());
	this->shaderProgram.setUniformVariable("pyramidColor", *color);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

}
